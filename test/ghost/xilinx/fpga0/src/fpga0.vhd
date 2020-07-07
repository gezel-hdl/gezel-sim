-- nallacode writetop2rx2tx

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity fpga0 is
	port( 
		clk100p     : in std_logic;
		clk100n     : in std_logic;
		reg_clk     : in std_logic;
		reg_reset_z : in std_logic;
		config_data : inout std_logic_vector(7 downto 0);
		reg_uds_z   : in std_logic;
		reg_lds_z   : in std_logic;
		reg_ads_z   : in std_logic;
		reg_en_z    : in std_logic;
		reg_rdy_z   : out std_logic;
		reg_rd_wr_z : in std_logic;

		lvds_rx_lane_p : in std_logic_vector(33 downto 0);
		lvds_rx_lane_n : in std_logic_vector(33 downto 0);
		lvds_rx_clk_p : in std_logic_vector(1 downto 0);
		lvds_rx_clk_n : in std_logic_vector(1 downto 0);
		lvds_tx_lane_p : out std_logic_vector(33 downto 0);
		lvds_tx_lane_n : out std_logic_vector(33 downto 0);
		lvds_tx_clk_p : out std_logic_vector(1 downto 0);
		lvds_tx_clk_n : out std_logic_vector(1 downto 0);

		eeprom_scl  : out std_logic;
		eeprom_sda  : inout std_logic;
		sys_led_out : out std_logic_vector(5 downto 0);
		ram_pwr_on  : out std_logic;
		ram_leds    : out std_logic_vector(1 downto 0);
		ram_pg      : in std_logic;
		mgt_pg      : in std_logic
		);
end fpga0;


architecture rtl of fpga0 is

	component comp_0_base_edge_2rx2tx
		port(
			clk100p : in STD_LOGIC;
			clk100n : in STD_LOGIC;
			reg_clk : in STD_LOGIC;
			reg_reset_z : in STD_LOGIC;
			config_data : inout STD_LOGIC_VECTOR(7 downto 0);
			reg_uds_z : in STD_LOGIC;
			reg_lds_z : in STD_LOGIC;
			reg_ads_z : in STD_LOGIC;
			reg_en_z : in STD_LOGIC;
			reg_rdy_z : out STD_LOGIC;
			reg_rd_wr_z : in STD_LOGIC;

			lvds_rx_lane_p : in STD_LOGIC_VECTOR(33 downto 0);
			lvds_rx_lane_n : in STD_LOGIC_VECTOR(33 downto 0);
			lvds_rx_clk_p : in STD_LOGIC_VECTOR(1 downto 0);
			lvds_rx_clk_n : in STD_LOGIC_VECTOR(1 downto 0);
			lvds_tx_lane_p : out STD_LOGIC_VECTOR(33 downto 0);
			lvds_tx_lane_n : out STD_LOGIC_VECTOR(33 downto 0);
			lvds_tx_clk_p : out STD_LOGIC_VECTOR(1 downto 0);
			lvds_tx_clk_n : out STD_LOGIC_VECTOR(1 downto 0);

			intra_mod_lvds_comms_control : inout STD_LOGIC_VECTOR(47 downto 0);
			upper_mod_lvds_comms_control : inout STD_LOGIC_VECTOR(47 downto 0);
			eeprom_scl : out STD_LOGIC;
			eeprom_sda : inout STD_LOGIC;
			sys_led_out : out STD_LOGIC_VECTOR(5 downto 0);
			ram_leds : out STD_LOGIC_VECTOR(1 downto 0);
			ram_pwr_on : out STD_LOGIC;
			ram_pg : in STD_LOGIC;
			mgt_pg : in STD_LOGIC;
			osc_clk : out STD_LOGIC;
			clk200mhz : out STD_LOGIC;
			clk200mhz_locked : out STD_LOGIC;

			user_reg_clk : out STD_LOGIC;
			user_interupt : in STD_LOGIC_VECTOR(3 downto 0);
			user_reg_wdata : out STD_LOGIC_VECTOR(15 downto 0);
			user_reg_addr : out STD_LOGIC_VECTOR(12 downto 0);
			user_reg_rden : out STD_LOGIC_VECTOR(3 downto 0);
			user_reg_wren : out STD_LOGIC_VECTOR(3 downto 0);
			user_reg_rdy : in STD_LOGIC_VECTOR(3 downto 0);
			user_reg_rdata0 : in STD_LOGIC_VECTOR(15 downto 0);
			user_reg_rdata1 : in STD_LOGIC_VECTOR(15 downto 0);
			user_reg_rdata2 : in STD_LOGIC_VECTOR(15 downto 0);
			user_reg_rdata3 : in STD_LOGIC_VECTOR(15 downto 0);

			clk : in STD_LOGIC;
			tx_data_valid : in STD_LOGIC;
			tx_data : in STD_LOGIC_VECTOR(255 downto 0);
			tx_data_almost_full : out STD_LOGIC;
			rx_data_read : in STD_LOGIC;
			rx_data_empty : out STD_LOGIC;
			rx_data_valid : out STD_LOGIC;
			rx_data : out STD_LOGIC_VECTOR(255 downto 0);
			sysmon_alarm : out STD_LOGIC_VECTOR(3 downto 0);
			leds : in STD_LOGIC_VECTOR(3 downto 0));
	end component;

	signal tx_data_valid : std_logic;
	signal tx_data : std_logic_vector(255 downto 0);
	signal tx_data_almost_full : std_logic;
	signal rx_data_valid : std_logic;
	signal rx_data : std_logic_vector(255 downto 0);
	signal rx_data_read: std_logic;
	signal rx_data_empty:std_logic;	 

	signal clk200mhz : std_logic;
	signal clk200mhz_locked : std_logic;
	alias  clk : std_logic is clk200mhz;	
        signal clk100 : std_logic;

	signal user_reg_clk : std_logic;
	signal user_interupt : std_logic_vector(3 downto 0);
	signal user_reg_wdata : std_logic_vector(15 downto 0);
	signal user_reg_addr : std_logic_vector(12 downto 0);
	signal user_reg_rden : std_logic_vector(3 downto 0);
	signal user_reg_wren : std_logic_vector(3 downto 0); 
	signal user_reg_rdy : std_logic_vector(3 downto 0);
	signal user_reg_rdata0 : std_logic_vector(15 downto 0);
	signal user_reg_rdata1 : std_logic_vector(15 downto 0);
	signal user_reg_rdata2 : std_logic_vector(15 downto 0);
	signal user_reg_rdata3 : std_logic_vector(15 downto 0);	

        signal user_reset : std_logic;

	signal sysmon_alarm : std_logic_vector(3 downto 0);
	signal leds : std_logic_vector(3 downto 0);

	signal intra_mod_lvds_comms_control:std_logic_vector(47 downto 0);
	signal upper_mod_lvds_comms_control:std_logic_vector(47 downto 0);

        -- GEZEL-generated SPL component
	component flowtru
	   port(
	       	CLK   : in std_logic; 
	       	RST   : in std_logic; 
		rx_data:in std_logic_vector(255 downto 0); 
		rx_data_valid:in std_logic;
		rx_data_empty:in std_logic;
		rx_data_read:out std_logic;
		tx_data:out std_logic_vector(255 downto 0);
		tx_data_valid:out std_logic;
		tx_data_almost_full:in std_logic); 
	end component;

-- system interconnect
begin
	comp_0_base_edge_2rx2tx_inst : comp_0_base_edge_2rx2tx
          port map(
	     clk100p          => clk100p,
	     clk100n          => clk100n,
	     reg_clk          => reg_clk,
	     reg_reset_z      => reg_reset_z,
	     config_data      => config_data,
	     reg_uds_z        => reg_uds_z,
	     reg_lds_z        => reg_lds_z,
	     reg_ads_z        => reg_ads_z,
	     reg_en_z         => reg_en_z,
	     reg_rdy_z        => reg_rdy_z,
	     reg_rd_wr_z      => reg_rd_wr_z,
	     lvds_rx_lane_p   => lvds_rx_lane_p,
	     lvds_rx_lane_n   => lvds_rx_lane_n,
	     lvds_rx_clk_p    => lvds_rx_clk_p,
	     lvds_rx_clk_n    => lvds_rx_clk_n,
	     lvds_tx_lane_p   => lvds_tx_lane_p,
	     lvds_tx_lane_n   => lvds_tx_lane_n,
	     lvds_tx_clk_p    => lvds_tx_clk_p,
	     lvds_tx_clk_n    => lvds_tx_clk_n,
	     intra_mod_lvds_comms_control=>intra_mod_lvds_comms_control,
	     upper_mod_lvds_comms_control=>upper_mod_lvds_comms_control,
	     eeprom_scl       => eeprom_scl,
	     eeprom_sda       => eeprom_sda,
	     sys_led_out      => sys_led_out,
	     ram_pwr_on       => ram_pwr_on,
	     ram_pg           => ram_pg,
	     mgt_pg           => mgt_pg,  
	     osc_clk          => clk100,
	     clk200mhz        => clk200mhz,
	     clk200mhz_locked => clk200mhz_locked, 
	     user_reg_clk     => user_reg_clk,
	     user_interupt    => user_interupt,
	     user_reg_wdata   => user_reg_wdata,
	     user_reg_addr    => user_reg_addr,
	     user_reg_rden    => user_reg_rden,
	     user_reg_wren    => user_reg_wren,	 
	     user_reg_rdy     => user_reg_rdy,
	     user_reg_rdata0  => user_reg_rdata0,
	     user_reg_rdata1  => user_reg_rdata1,
	     user_reg_rdata2  => user_reg_rdata2,
	     user_reg_rdata3  => user_reg_rdata3,
	     clk              => clk,
	     tx_data_valid    => tx_data_valid,
	     tx_data          => tx_data,
	     tx_data_almost_full => tx_data_almost_full,
	     rx_data_valid    => rx_data_valid,	
	     rx_data_read     => rx_data_read,
	     rx_data_empty    => rx_data_empty,
	     rx_data          => rx_data,
	     sysmon_alarm     => sysmon_alarm,
	     leds             => leds,  
	     ram_leds         => ram_leds
	     );
        upper_mod_lvds_comms_control(46 downto 30)<=(others=>'0');
	intra_mod_lvds_comms_control(46 downto 30)<=(others=>'0');

	 UserSPL_flowtru : flowtru
	   port map(
	       	CLK           => clk,
	       	RST           => user_reset,
		rx_data       => rx_data,
		rx_data_valid => rx_data_valid,
		rx_data_empty => rx_data_empty,
		rx_data_read  => rx_data_read,
		tx_data       => tx_data,
		tx_data_valid => tx_data_valid,
		tx_data_almost_full => tx_data_almost_full
		);

        user_reset <= '0';
        user_reg_rdy(0) <= '0';
        user_interupt(0) <= '0';
        user_reg_rdata0 <= (others=>'0');
        user_reg_rdy(1) <= '0';
        user_interupt(1) <= '0';
        user_reg_rdata1 <= (others=>'0');
        user_reg_rdy(2) <= '0';
        user_interupt(2) <= '0';
        user_reg_rdata2 <= (others=>'0');
        user_reg_rdy(3) <= '0';
        user_interupt(3) <= '0';
        user_reg_rdata3 <= (others=>'0');
        leds <= (others=>'0');
end rtl;
