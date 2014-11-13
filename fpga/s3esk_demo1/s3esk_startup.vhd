-- 10/05/07 RP/LGM
--		- port COM tx 
--
-- 11/05/07 RP/LGM
-- 	- s3esk initial design 
--		- integre composant encodeur rotatif + conversion BCD accessible
--				01 datah 
--				03 datal
--				   afficheur LCD 

--
--
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--
------------------------------------------------------------------------------------
--
--
entity s3esk_startup is
    Port (             led : out std_logic_vector(7 downto 0);
            strataflash_oe : out std_logic;
            strataflash_ce : out std_logic;
            strataflash_we : out std_logic;
                    switch : in std_logic_vector(3 downto 0);
                 btn_north : in std_logic;
                  btn_east : in std_logic;
                 btn_south : in std_logic;
                  btn_west : in std_logic;
                     lcd_d : inout std_logic_vector(7 downto 4);
                    lcd_rs : out std_logic;
                    lcd_rw : out std_logic;
                     lcd_e : out std_logic;
                  rotary_a : in std_logic;
                  rotary_b : in std_logic;
              rotary_press : in std_logic;
								tx : out std_logic;
								--rx : in std_logic;
                       clk : in std_logic);
    end s3esk_startup;
--
------------------------------------------------------------------------------------
--
-- Start of test architecture
--
architecture Behavioral of s3esk_startup is
--
------------------------------------------------------------------------------------
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
	 
	 
-- quadrature counter
	component counter 
    Port ( qA : in  STD_LOGIC;
           qB : in  STD_LOGIC;
           clk : in  STD_LOGIC;
           data : out  STD_LOGIC_VECTOR (15 downto 0)
			 );
	end component;

-- uart tx
	component uart_tx 
    Port (            data_in : in std_logic_vector(7 downto 0);
                 write_buffer : in std_logic;
                 reset_buffer : in std_logic;
                 en_16_x_baud : in std_logic;
                   serial_out : out std_logic;
                  buffer_full : out std_logic;
             buffer_half_full : out std_logic;
                          clk : in std_logic
			);
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

--count rotations
signal        data      : std_logic_vector(15 downto 0);
signal        datah      : std_logic_vector(7 downto 0);
signal        datal      : std_logic_vector(7 downto 0);

-- RS232 COM
signal          baud_count : integer range 0 to 127 :=0;
signal        en_16_x_baud : std_logic;
signal       write_to_uart : std_logic;
signal             tx_full : std_logic;
signal        tx_half_full : std_logic;
signal           uart_data : std_logic_vector(7 downto 0);

--
--
------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--
-- Start of circuit description
--
begin
  --
  ----------------------------------------------------------------------------------------------------------------------------------
  -- Disable unused components  
  ----------------------------------------------------------------------------------------------------------------------------------
  --
  --StrataFLASH must be disabled to prevent it conflicting with the LCD display 
  --
  strataflash_oe <= '1';
  strataflash_ce <= '1';
  strataflash_we <= '1';
 
 ----------------------------------------------------------------------------------------------------------------------------------
  -- KCPSM3 and the program memory 
  ----------------------------------------------------------------------------------------------------------------------------------
  --

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
 
  program_rom: control
    port map(      address => address,
               instruction => instruction,
                proc_reset => kcpsm3_reset,                       --JTAG Loader version 
                       clk => clk);

	c: counter 
    port map(
	        qA   => rotary_a, 
           qB   => rotary_b, 
           clk	 => clk,  
           data => data 
			  );

	t: uart_tx 
    Port map (          data_in => uart_data, -- donnée à sérialiser
                   write_buffer => write_to_uart,
                   reset_buffer => '0',
                   en_16_x_baud => en_16_x_baud,
                     serial_out => tx,
                    buffer_full => tx_full,
               buffer_half_full => tx_half_full,
                            clk => clk  				  
					);
					
baud_timer: process(clk)
  begin
    if clk'event and clk='1' then
      if baud_count=81 then
           baud_count <= 0;
         en_16_x_baud <= '1';
       else
           baud_count <= baud_count + 1;
         en_16_x_baud <= '0';
      end if;
    end if;
  end process baud_timer;  
  
  --
  ----------------------------------------------------------------------------------------------------------------------------------
  -- KCPSM3 input ports 
  ----------------------------------------------------------------------------------------------------------------------------------
  --
  --
  -- The inputs connect via a pipelined multiplexer
  --

  datah <= data(15 downto  8);
  datal <= data(7 downto  0);

  input_ports: process(clk)
  begin
    if clk'event and clk='1' then

      case port_id(7 downto 0) is

 
		  -- read quadrature counter
		  when "00000001" => 	in_port <= datah;		
		  when "00000011" => 	in_port <= datal;
		  
		  -- read uart status register
        when "00000000" =>    in_port <= "000000" & tx_full & tx_half_full;

        -- read LCD data at address 02 hex
        when "00000010" =>    in_port <= lcd_d & "0000";

        -- Don't care used for all other addresses to ensure minimum logic implementation
        when others =>    in_port <= "XXXXXXXX";  

      end case;

     end if;

  end process input_ports;


  --
  ----------------------------------------------------------------------------------------------------------------------------------
  -- KCPSM3 output ports 
  ----------------------------------------------------------------------------------------------------------------------------------
  --

  -- adding the output registers to the processor
   
  output_ports: process(clk)
  begin

    if clk'event and clk='1' then
      if write_strobe='1' then

        -- Write to LEDs at address 80 hex.
        if port_id(7)='1' then
          led <= out_port;
        end if;

        -- LCD data output and controls at address 40 hex.
        if port_id(6)='1' then
          lcd_output_data <= out_port(7 downto 4);
          lcd_drive <= out_port(3);  
          lcd_rs <= out_port(2);
          lcd_rw_control <= out_port(1);
          lcd_e <= out_port(0);
        end if;

		 -- UART tx data
		 if port_id(5)='1' then
			 uart_data <= out_port;	 
		 end if;

      end if;

    end if; 

  end process output_ports;

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

  --uart interface
  write_to_uart <= write_strobe and port_id(5);

end Behavioral;

------------------------------------------------------------------------------------------------------------------------------------
--
-- END OF FILE s3esk_startup.vhd
--
------------------------------------------------------------------------------------------------------------------------------------

