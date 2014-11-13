-- Standard IEEE libraries
--
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library unisim;
use unisim.VComponents.all;


library techmap;
use techmap.gencomp.all;
library gaisler;
use gaisler.all;


entity cantest is
    Port (   
				led : out std_logic_vector(7 downto 0);
	 
            strataflash_oe : out std_logic;
            strataflash_ce : out std_logic;
            strataflash_we : out std_logic;
				
                    switch : in std_logic_vector(3 downto 0);
                 
                     lcd_d : inout std_logic_vector(7 downto 4);
                    lcd_rs : out std_logic;
                    lcd_rw : out std_logic;
                     lcd_e : out std_logic;
                  
                       clk : in std_logic;
							  
							  rx_pin_p : in std_logic;  rx_pin_n : in std_logic;
							  tx_pin_p : out std_logic; tx_pin_n : out std_logic

--							rx_pin : in std_logic;
--							tx_pin : out std_logic

							  );
    end cantest;
--
------------------------------------------------------------------------------------
--
-- Start of test architecture
--
architecture Behavioral of cantest is
--

--gaisler grlib CAN2B
---------------------
component can_mod
	 generic ( memtech   : integer );
    port(
      reset   : in  std_logic;        
      clk     : in  std_logic;        
      cs      : in  std_logic;        
      we      : in  std_logic;        
      addr    : in  std_logic_vector(7 downto 0);   
      data_in : in  std_logic_vector(7 downto 0);   
      data_out: out std_logic_vector(7 downto 0);   
      irq     : out std_logic;      
      rxi     : in  std_logic;      
      txo     : out std_logic    
    );
  end component;

--
-- declaration of KCPSM3
--
  component kcpsm3 
    Port (      address : out std_logic_vector(9 downto 0);
            instruction : in std_logic_vector(17 downto 0);
				
                port_id : out std_logic_vector(7 downto 0);
           write_strobe : out std_logic;
               out_port : out std_logic_vector(7 downto 0);
            read_strobe : out std_logic;
                in_port : in std_logic_vector(7 downto 0);
					 
              interrupt : in std_logic;
          interrupt_ack : out std_logic;
			 
                  reset : in std_logic;
                    clk : in std_logic);
    end component;
--
-- declaration of program ROM
--
  component control
    Port (      address : in std_logic_vector(9 downto 0);
            instruction : out std_logic_vector(17 downto 0);
             proc_reset : out std_logic;                       --JTAG Loader version
                    clk : in std_logic);
    end component;
	  
	component OBUFDS
		port (O : out STD_ULOGIC;
				OB : out STD_ULOGIC;
				I : in STD_ULOGIC);
	end component; 
	
	
	component IBUFDS
		generic (
			IOSTANDARD : string ;
			DIFF_TERM : string);
		port (
			O : out STD_ULOGIC;  
			I : in STD_ULOGIC;  
			IB : in STD_ULOGIC);  
	end component;
	
-- Signals used to connect KCPSM3 to program ROM and I/O logic
signal address          : std_logic_vector(9 downto 0);
signal instruction      : std_logic_vector(17 downto 0);
signal port_id          : std_logic_vector(7 downto 0);
signal out_port         : std_logic_vector(7 downto 0);
signal in_port          : std_logic_vector(7 downto 0);
signal write_strobe     : std_logic;
signal read_strobe      : std_logic;
signal interrupt        : std_logic :='0';
signal interrupt_ack    : std_logic;
signal kcpsm3_reset     : std_logic;
signal tx_pin     		: std_logic;
signal rx_pin     		: std_logic;

-- Signals for LCD operation
signal   lcd_rw_control : std_logic;
signal  lcd_output_data : std_logic_vector(7 downto 4);
signal        lcd_drive : std_logic;

--signal for CAN2B
signal  		 addr	:  std_logic_vector(7 downto 0);
signal  	data_out	:  std_logic_vector(7 downto 0);
signal  	 data_in :  std_logic_vector(7 downto 0);
signal     can_cs :  std_logic;
signal     can_we :  std_logic;
signal        irq :  std_logic;

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--
-- Start of circuit description
--
begin
  can: can_mod
    generic map (memtech => inferred)	--generic ram
    port map (
		  reset  => kcpsm3_reset,              
        addr => addr,              
        data_in  => data_in,          
        data_out => data_out,           
        cs => can_cs,              
        we => can_we,             
        clk => clk ,          
        rxi  => rx_pin,          
        txo  => tx_pin,
		  irq => irq);

 o_b : OBUFDS
      port map (O => tx_pin_p,
                OB => tx_pin_n,
                I => tx_pin); 
 
 i_b : IBUFDS
   generic map (
      IOSTANDARD => "LVDS_25",
		DIFF_TERM => "TRUE" )		
   port map (
      O => rx_pin,  
      I => rx_pin_p,  
      IB => rx_pin_n 
   );  
	
processor: kcpsm3
    port map(      address => address,
               instruction => instruction,
                   port_id => port_id,
              write_strobe => write_strobe,
                  out_port => out_port,
               read_strobe => read_strobe,
                   in_port => in_port,
                 interrupt => interrupt,
             interrupt_ack => interrupt_ack,
                     reset => kcpsm3_reset,
                       clk => clk);
 
  control_rom: control
    port map(      address => address,
               instruction => instruction,
                proc_reset => kcpsm3_reset,                   --JTAG Loader version 
                       clk => clk);

-- The inputs connect via a pipelined multiplexer
  input_ports: process(clk)
  begin
    if clk'event and clk='1' then
	 --if read_strobe='1' then NOT OW with this statement !!!
      case port_id(7 downto 6) is	  
        --read switch data
		  when "00" =>  null;  
		    in_port <= "0000" & switch;		  
        -- read LCD data at address 10xx xxxx
        when "10" =>    null;
		    in_port <= lcd_d & "0000";	  
		  -- read can data at address 11xx aaaa
        when "11" =>    
			 in_port <= data_out;				 
        -- Don't care used for all other addresses to ensure minimum logic implementation
        when others =>  in_port <= "XXXXXXXX";  
      end case;
		end if;
    -- end if;
  end process input_ports;

-- adding the output registers to the processor 
  output_ports: process(clk)
  begin
    if clk'event and clk='1' then
      if write_strobe='1' then     
		case port_id(7 downto 6) is
		  -- Write to LEDs at address 80 hex.
        when "10" => 
		    led <= out_port; 			 			 
        -- LCD data output and controls at address 40 hex.
        when "01" =>  
			 lcd_output_data <= out_port(7 downto 4);
          lcd_drive <= out_port(3);  
          lcd_rs <= out_port(2);
          lcd_rw_control <= out_port(1);
          lcd_e <= out_port(0);
		-- CANB data
		  when "11" => 
			-- data_in <= out_port; --NOT OK !!! see below
			 null;
		  when others => null; 	 			 
		  end case;		
		end if;
    end if; 
  end process output_ports;

--can addr interface 32 registers
  can_cs <='1';
  can_we <='1' when write_strobe='1' and port_id(7 downto 6)="11" else '0';
  addr(7 downto 0) <= "00" & port_id(5 downto 0);
  data_in <= out_port when write_strobe='1' and port_id(7 downto 6)="11" else "XXXXXXXX"; --OK
  
  -- LCD interface  
  lcd_rw <= lcd_rw_control and lcd_drive;
  lcd_d <= lcd_output_data when (lcd_rw_control='0' and lcd_drive='1') else "ZZZZ";
  --StrataFLASH must be disabled to prevent it conflicting with the LCD display 
  strataflash_oe <= '1';
  strataflash_ce <= '1';
  strataflash_we <= '1';
end Behavioral;


