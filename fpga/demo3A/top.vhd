----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:00:37 09/26/2007 
-- Design Name: 
-- Module Name:    top - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: this is a sample for testing purpose
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

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity top is
    Port ( clk : in  STD_LOGIC;
           sw0 : in  STD_LOGIC;
           sw1 : in  STD_LOGIC;
           led : out  STD_LOGIC_VECTOR (7 downto 0));
end top;

architecture Behavioral of top is

signal cpt : STD_LOGIC_VECTOR (27 downto 0);

begin

process
begin
	if rising_edge(clk) then
		cpt <= cpt + 1;
		end if;
end process;
	
led <= cpt(27 downto 20);

end Behavioral;

