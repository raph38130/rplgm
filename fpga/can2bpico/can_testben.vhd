-- Standard IEEE libraries
--
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity can_tb is
    
    end can_tb;
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
--
------------------------------------------------------------------------------------
--
-- Signals used to connect KCPSM3 to program ROM and I/O logic
--
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
--
--
-- Signals for LCD operation
--
-- Tri-state output requires internal signals
-- 'lcd_drive' is used to differentiate between LCD and StrataFLASH communications 
-- which share the same data bits.
--
signal   lcd_rw_control : std_logic;
signal  lcd_output_data : std_logic_vector(7 downto 4);
signal        lcd_drive : std_logic;
--
--

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
  --StrataFLASH must be disabled to prevent it conflicting with the LCD display 
  --
  strataflash_oe <= '1';
  strataflash_ce <= '1';
  strataflash_we <= '1';

  can: can_mod
    generic map (memtech => 0)	--generic ram
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
  --
  input_ports: process(clk)
  begin
    if clk'event and clk='1' then
	 if read_strobe='1' then
      case port_id(7 downto 6) is
		  
        --read switch data
		  when "00" =>    
		    in_port <= "0000" & switch;
		  
        -- read LCD data at address 10xx xxxx
        when "10" =>    
		    in_port <= lcd_d & "0000";
		  
		  -- read can data at address 11xx aaaa
        when "11" =>    
			 in_port <= data_out;						

        -- Don't care used for all other addresses to ensure minimum logic implementation
        when others =>  in_port <= "XXXXXXXX";  
      end case;
		end if;
     end if;
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
          --can_we <='0';
			 
        -- LCD data output and controls at address 40 hex.
        when "01" =>  
			 lcd_output_data <= out_port(7 downto 4);
          lcd_drive <= out_port(3);  
          lcd_rs <= out_port(2);
          lcd_rw_control <= out_port(1);
          lcd_e <= out_port(0);
          --can_we <= '0';	
			 
	     -- CANB data
		  when "11" => 
			 data_in <= out_port; 
		    --can_we <= '1'; 
		  	 
		  when others => null;
--			 can_we <= '0'; 	 
			 
		  end case;		
		end if;
    end if; 
  end process output_ports;

  --can addr interface 32 registers
  addr(7 downto 0) <= "00" & port_id(5 downto 0);
  can_cs <= '1';
  can_we <= '1' when write_strobe='1' and port_id(7 downto 6)="11" else '0';
  --
  ----------------------------------------------------------------------------------------------------------------------------------
  -- LCD interface  
  ----------------------------------------------------------------------------------------------------------------------------------
  --
  -- The 4-bit data port is bidirectional.
  -- lcd_rw is '1' for read and '0' for write 
  -- lcd_drive is like a master enable signal which prevents either the 
  -- FPGA outputs or the LCD display driving the data lines.
  --
  --Control of read and write signal
  lcd_rw <= lcd_rw_control and lcd_drive;

  --use read/write control to enable output buffers.
  lcd_d <= lcd_output_data when (lcd_rw_control='0' and lcd_drive='1') else "ZZZZ";
 
end Behavioral;


