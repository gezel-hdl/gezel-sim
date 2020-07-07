library ieee;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.std_logic_arithext.all;


--datapath entity
entity flowtru is
port(
   rx_data:in std_logic_vector(255 downto 0);
   rx_data_valid:in std_logic;
   rx_data_empty:in std_logic;
   rx_data_read:out std_logic;
   tx_data:out std_logic_vector(255 downto 0);
   tx_data_valid:out std_logic;
   tx_data_almost_full:in std_logic;
   RST : in std_logic;
   CLK : in std_logic

);
end flowtru;


--signal declaration
architecture RTL of flowtru is
signal r_tx_data:std_logic_vector(255 downto 0);
signal r_tx_data_wire:std_logic_vector(255 downto 0);
signal r_rx_data_read:std_logic;
signal r_rx_data_read_wire:std_logic;
signal rr_rx_data_read:std_logic;
signal rr_rx_data_read_wire:std_logic;
signal sum1:std_logic_vector(31 downto 0);
signal sum2:std_logic_vector(31 downto 0);
signal sum3:std_logic_vector(31 downto 0);
signal sum4:std_logic_vector(31 downto 0);
signal sum5:std_logic_vector(31 downto 0);
signal sum6:std_logic_vector(31 downto 0);
signal sum7:std_logic_vector(31 downto 0);
signal sum8:std_logic_vector(31 downto 0);
signal sig_0:std_logic;
signal sig_1:std_logic;
signal sig_2:std_logic;
signal sig_3:std_logic;
signal sig_4:std_logic;
signal sig_5:std_logic_vector(31 downto 0);
signal sig_6:std_logic_vector(31 downto 0);
signal sig_7:std_logic_vector(31 downto 0);
signal sig_8:std_logic_vector(31 downto 0);
signal sig_9:std_logic_vector(31 downto 0);
signal sig_10:std_logic_vector(31 downto 0);
signal sig_11:std_logic_vector(31 downto 0);
signal sig_12:std_logic_vector(31 downto 0);
signal sig_13:std_logic_vector(63 downto 0);
signal sig_14:std_logic_vector(95 downto 0);
signal sig_15:std_logic_vector(127 downto 0);
signal sig_16:std_logic_vector(159 downto 0);
signal sig_17:std_logic_vector(191 downto 0);
signal sig_18:std_logic_vector(223 downto 0);
signal sig_19:std_logic_vector(255 downto 0);
signal tx_data_valid_int:std_logic;
signal rx_data_read_int:std_logic;
signal tx_data_int:std_logic_vector(255 downto 0);


begin
--register updates
dpREG: process (CLK,RST)
   begin
      if (RST = '1') then
         r_tx_data <= (others=>'0');
         r_rx_data_read <= '0';
         rr_rx_data_read <= '0';
      elsif CLK' event and CLK = '1' then
         r_tx_data <= r_tx_data_wire;
         r_rx_data_read <= r_rx_data_read_wire;
         rr_rx_data_read <= rr_rx_data_read_wire;

      end if;
   end process dpREG;


--combinational logics
dpCMB: process (r_tx_data,r_rx_data_read,rr_rx_data_read,sum1,sum2,sum3,sum4,sum5,sum6,sum7
,sum8,sig_0,sig_1,sig_2,sig_3,sig_4,sig_5,sig_6,sig_7,sig_8
,sig_9,sig_10,sig_11,sig_12,sig_13,sig_14,sig_15,sig_16,sig_17,sig_18
,sig_19,tx_data_valid_int,rx_data_read_int,tx_data_int,rx_data,rx_data_valid,rx_data_empty,tx_data_almost_full)
   begin
      r_tx_data_wire <= r_tx_data;
      r_rx_data_read_wire <= r_rx_data_read;
      rr_rx_data_read_wire <= rr_rx_data_read;
      sum1 <= (others=>'0');
      sum2 <= (others=>'0');
      sum3 <= (others=>'0');
      sum4 <= (others=>'0');
      sum5 <= (others=>'0');
      sum6 <= (others=>'0');
      sum7 <= (others=>'0');
      sum8 <= (others=>'0');
      sig_0 <= '0';
      sig_1 <= '0';
      sig_2 <= '0';
      sig_3 <= '0';
      sig_4 <= '0';
      sig_5 <= (others=>'0');
      sig_6 <= (others=>'0');
      sig_7 <= (others=>'0');
      sig_8 <= (others=>'0');
      sig_9 <= (others=>'0');
      sig_10 <= (others=>'0');
      sig_11 <= (others=>'0');
      sig_12 <= (others=>'0');
      sig_13 <= (others=>'0');
      sig_14 <= (others=>'0');
      sig_15 <= (others=>'0');
      sig_16 <= (others=>'0');
      sig_17 <= (others=>'0');
      sig_18 <= (others=>'0');
      sig_19 <= (others=>'0');
      tx_data_valid_int <= '0';
      rx_data_read_int <= '0';
      tx_data_int <= (others=>'0');
      rx_data_read <= '0';
      tx_data <= (others=>'0');
      tx_data_valid <= '0';

      sig_0 <=  not rx_data_empty;
      sig_1 <=  not tx_data_almost_full;
      sig_2 <= sig_0 and sig_1;
      sig_3 <=  not r_rx_data_read;
      sig_4 <= sig_2 and sig_3;
      tx_data_valid <= tx_data_valid_int;
      rx_data_read <= rx_data_read_int;
      sig_5 <= unsigned(rx_data(31 downto 0)) + unsigned(conv_std_logic_vector(1,32));
      sum1 <= sig_5;
      sig_6 <= unsigned(rx_data(63 downto 32)) + unsigned(conv_std_logic_vector(2,32));
      sum2 <= sig_6;
      sig_7 <= unsigned(rx_data(95 downto 64)) + unsigned(conv_std_logic_vector(3,32));
      sum3 <= sig_7;
      sig_8 <= unsigned(rx_data(127 downto 96)) + unsigned(conv_std_logic_vector(4,32));
      sum4 <= sig_8;
      sig_9 <= unsigned(rx_data(159 downto 128)) + unsigned(conv_std_logic_vector(5,32));
      sum5 <= sig_9;
      sig_10 <= unsigned(rx_data(191 downto 160)) + unsigned(conv_std_logic_vector(6,32));
      sum6 <= sig_10;
      sig_11 <= unsigned(rx_data(223 downto 192)) + unsigned(conv_std_logic_vector(7,32));
      sum7 <= sig_11;
      sig_12 <= unsigned(rx_data(255 downto 224)) + unsigned(conv_std_logic_vector(8,32));
      sum8 <= sig_12;
      sig_13 <= sum8 & sum7;
      sig_14 <= sig_13 & sum6;
      sig_15 <= sig_14 & sum5;
      sig_16 <= sig_15 & sum4;
      sig_17 <= sig_16 & sum3;
      sig_18 <= sig_17 & sum2;
      sig_19 <= sig_18 & sum1;
      tx_data <= tx_data_int;
      tx_data_valid_int <= rr_rx_data_read;
      rx_data_read_int <= r_rx_data_read;
      tx_data_int <= r_tx_data;
      r_rx_data_read_wire <= sig_4;
      rr_rx_data_read_wire <= r_rx_data_read;
      r_tx_data_wire <= sig_19;
   end process dpCMB;
end RTL;
