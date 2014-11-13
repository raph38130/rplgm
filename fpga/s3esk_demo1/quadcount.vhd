----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:37:00 04/30/2007 
-- Design Name: 
-- Module Name:    top - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity counter is
    Port ( qA : in  STD_LOGIC;
           qB : in  STD_LOGIC;
           clk : in  STD_LOGIC;
           data : out  STD_LOGIC_VECTOR (15 downto 0));
end counter;


-- Start of test architecture
--
architecture Behavioral of counter is

signal      rotary_a_in : std_logic;
signal      rotary_b_in : std_logic;
signal  rotary_press_in : std_logic;
signal        rotary_in : std_logic_vector(1 downto 0);
signal        rotary_q1 : std_logic;
signal        rotary_q2 : std_logic;
signal  delay_rotary_q1 : std_logic;
signal     rotary_event : std_logic;
signal      rotary_left : std_logic;
signal      cpt : std_logic_vector(15 downto 0):= "0000000000000000"; --initial value puts one LED on near the middle.

signal      bcdval : std_logic_vector(15 downto 0):= "0000000000000000"; --initial value puts one LED on near the middle.
signal      init : std_logic;
signal      modin : std_logic;
signal      i : std_logic_vector(3 downto 0):="1111";
signal      cpttest : std_logic_vector(15 downto 0):= "0000010011010010"; --initial value puts one LED on near the middle.


component BCDConv 
  generic (N   : positive);     -- number of digits
  port (Clock  : in std_logic;
        Reset  : in std_logic;
        Init   : in std_logic;  -- initialise conversion
        ModIn  : in std_logic;  -- carry in from outside
        ModOut : out std_logic; -- carry out 
        Q      : out std_logic_vector(4*N -1 downto 0) -- BCD result
       );
end component;

-- Start of circuit description
--
begin
  --
  process(clk) 
		  begin 
		  if rising_edge(clk) then
				if i="1111"
					then init <='1'; 
					else init <='0'; 
					end if; 
				modin <= cpt(conv_integer(i));
				i <= i-1;
				end if;
			end process;
  
  process(clk)
  begin
	  if rising_edge(clk) then
	  if init='1' then 
			data(3 downto 0) <= bcdval(15 downto 12); 
			data(7 downto 4) <= bcdval(11 downto 8); 
			data(11 downto 8) <= bcdval(7 downto 4); 
			data(15 downto 12) <= bcdval(3 downto 0); 
			end if;
	  end if;
  end process;
  
  
  conv: BCDConv 
		  generic map (N => 4)     
		  port map(Clock  => clk,
				  Reset  => rotary_press_in,
				  Init   => init,
				  ModIn  => modin,
				  ModOut => open,
				  Q      => bcdval
				 );

  rotary_filter: process(clk)
  begin
    if clk'event and clk='1' then

      --Synchronise inputs to clock domain using flip-flops in input/output blocks.
      rotary_a_in <= qA;
      rotary_b_in <= qB;
      rotary_press_in <= '0';

      --concatinate rotary input signals to form vector for case construct.
      rotary_in <= rotary_b_in & rotary_a_in;

      case rotary_in is

        when "00" => rotary_q1 <= '0';         
                     rotary_q2 <= rotary_q2;
 
        when "01" => rotary_q1 <= rotary_q1;
                     rotary_q2 <= '0';

        when "10" => rotary_q1 <= rotary_q1;
                     rotary_q2 <= '1';

        when "11" => rotary_q1 <= '1';
                     rotary_q2 <= rotary_q2; 

        when others => rotary_q1 <= rotary_q1; 
                       rotary_q2 <= rotary_q2; 
      end case;

    end if;
  end process rotary_filter;
  --
  -- The rising edges of 'rotary_q1' indicate that a rotation has occurred and the 
  -- state of 'rotary_q2' at that time will indicate the direction. 
  --
  direction: process(clk)
  begin
    if clk'event and clk='1' then

      delay_rotary_q1 <= rotary_q1;
      if rotary_q1='1' and delay_rotary_q1='0' then
        rotary_event <= '1';
        rotary_left <= rotary_q2;
       else
        rotary_event <= '0';
        rotary_left <= rotary_left;
      end if;

    end if;
  end process direction;
  --
  --
  ----------------------------------------------------------------------------------------------------------------------------------
  -- LED control.
  ----------------------------------------------------------------------------------------------------------------------------------
  --
  -- The LED pattern is provided by a shift register which has been initialised in the signal 
  -- definition to have only one bit active (High). 
  --
  led_display: process(clk)
  begin
    if clk'event and clk='1' then
      if rotary_event='1' then
        if rotary_left='1' then 
          cpt <= cpt + 1; --rotate LEDs to left
         else
          cpt <= cpt - 1; --rotate LEDs to right
        end if;
      end if;
     --
     -- Pressing the rotary encoder will cause cpt 0
     --
      if rotary_press_in='1' then
        cpt <= (others => '0');   
      end if;  

    end if;
  end process ;
 
 --data <= cpt;

end Behavioral;

