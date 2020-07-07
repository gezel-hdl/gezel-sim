//--------------------------------------------------------------
// Copyright (C) 2003-2010 P. Schaumont 
//                                                                             
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//                                                                             
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                             
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
//--------------------------------------------------------------

#include "nallasim.h"
#include <stdlib.h>

nallasim::nallasim(string file) {
  char *n = (char *) file.c_str();
  if (call_parser(n)) {
    cout << "Failed to parse " << file << "\n";
    exit(0);
  }

  glbAllowSimulatorSleep = 0;
  glbUseGVALINT = 0;
  glbIpblockcreate = stdipblockcreate;

  ofstream F("fdl.symbols");
  glbSymboltable.show(F);
  F.close();

  glbSymboltable.createcode(&RTLsim);
  RTLsim.reset();


}

void nallasim::ReadUserRegister(unsigned sock, 
				unsigned itf, 
				unsigned addr, 
				unsigned *data) {

  map<nalla_regio *, unsigned>::iterator i;

  for (i = nalla_regio_socket_map.begin(); i != nalla_regio_socket_map.end(); i++) {
    // test if sockets match
    if (i->second == sock) {
      // check if interface map
      if (nalla_regio_interface_map[i->first] == itf) {
	// simulate RTL until read operation completes
	while (i->first->ext_read(addr, data) == false)
	  RTLsim.advance(1);
      }
    }
  }

}

void nallasim::WriteUserRegister(unsigned sock, 
				 unsigned itf, 
				 unsigned addr, 
				 unsigned data) {

  map<nalla_regio *, unsigned>::iterator i;

  for (i = nalla_regio_socket_map.begin(); i != nalla_regio_socket_map.end(); i++) {
    // test if sockets match
    if (i->second == sock) {
      // check if interface map
      if (nalla_regio_interface_map[i->first] == itf) {
	// simulate RTL until write operation completes
	while (i->first->ext_write(addr, data) == false)
	  RTLsim.advance(1);
      }
    }
  }
  
}


void nallasim::MemCopy(unsigned sock,
                       unsigned *source, unsigned sourcebytes,
                       unsigned *dest,   unsigned destbytes) {

  bool doingwrite, doingread;
  map<nalla_spl *, unsigned>::iterator i;

  for (i = nalla_spl_socket_map.begin(); i != nalla_spl_socket_map.end(); i++) {
    //    cout << "**\n";
    // test if sockets match
    if (i->second == sock) {
      i->first->write_spl_init(source, sourcebytes);
      i->first->read_spl_init(dest, destbytes);
      // simulate RTL until memcopy completes
      //      cout << "sim loop start\n";
      doingwrite = true;
      doingread = true;
      do {
	doingwrite = doingwrite and not i->first->write_spl_check(source, sourcebytes);
	doingread  = doingread and not i->first->read_spl_check(dest, destbytes);
	RTLsim.advance(1);
	//cout << " done all " << done << "\n";
      } while (doingwrite or doingread);
    }
  }
}


//---------------------------------
nallacode::nallacode(enum sysedgetype p) : edge(p) {
  splcell = "userlogic"; // default name for spl cell
  clkdiv  = false;
}

void nallacode::adduser(unsigned channel, string cell) {
  userchannel[channel] = cell;
}

void nallacode::addspl(string cell) {
  splcell = cell;
}

void nallacode::divideclock() {
  clkdiv = true;
}

void nallacode::writetop0rx0tx(ostream &F, string tcn) {
  F << "-- nallacode writetop0rx0tx\n";
  F << "\n";
  F << "library IEEE;\n";
  F << "use IEEE.STD_LOGIC_1164.all;\n";
  F << "\n";
  F << "entity " << tcn << " is\n";
  F << "	port( \n";
  F << "		clk100p     : in std_logic;\n";
  F << "		clk100n     : in std_logic;\n";
  F << "		reg_clk     : in std_logic;\n";
  F << "		reg_reset_z : in std_logic;\n";
  F << "		config_data : inout std_logic_vector(7 downto 0);\n";
  F << "		reg_uds_z   : in std_logic;\n";
  F << "		reg_lds_z   : in std_logic;\n";
  F << "		reg_ads_z   : in std_logic;\n";
  F << "		reg_en_z    : in std_logic;\n";
  F << "		reg_rdy_z   : out std_logic;\n";
  F << "		reg_rd_wr_z : in std_logic;\n";
  F << "\n";
  F << "		eeprom_scl  : out std_logic;\n";
  F << "		eeprom_sda  : inout std_logic;\n";
  F << "		sys_led_out : out std_logic_vector(5 downto 0);\n";
  F << "		ram_pwr_on  : out std_logic;\n";
  F << "		ram_leds    : out std_logic_vector(1 downto 0);\n";
  F << "		ram_pg      : in std_logic;\n";
  F << "		mgt_pg      : in std_logic\n";
  F << "		);\n";	
  F << "end " << tcn << ";\n";
  F << "\n";
}

void nallacode::writetop2rx2tx(ostream &F, string tcn) {
  F << "-- nallacode writetop2rx2tx\n";
  F << "\n";
  F << "library IEEE;\n";
  F << "use IEEE.STD_LOGIC_1164.all;\n";
  F << "\n";
  F << "entity " << tcn << " is\n";
  F << "	port( \n";
  F << "		clk100p     : in std_logic;\n";
  F << "		clk100n     : in std_logic;\n";
  F << "		reg_clk     : in std_logic;\n";
  F << "		reg_reset_z : in std_logic;\n";
  F << "		config_data : inout std_logic_vector(7 downto 0);\n";
  F << "		reg_uds_z   : in std_logic;\n";
  F << "		reg_lds_z   : in std_logic;\n";
  F << "		reg_ads_z   : in std_logic;\n";
  F << "		reg_en_z    : in std_logic;\n";
  F << "		reg_rdy_z   : out std_logic;\n";
  F << "		reg_rd_wr_z : in std_logic;\n";
  F << "\n";
  F << "		lvds_rx_lane_p : in std_logic_vector(33 downto 0);\n";
  F << "		lvds_rx_lane_n : in std_logic_vector(33 downto 0);\n";
  F << "		lvds_rx_clk_p : in std_logic_vector(1 downto 0);\n";
  F << "		lvds_rx_clk_n : in std_logic_vector(1 downto 0);\n";
  F << "		lvds_tx_lane_p : out std_logic_vector(33 downto 0);\n";
  F << "		lvds_tx_lane_n : out std_logic_vector(33 downto 0);\n";
  F << "		lvds_tx_clk_p : out std_logic_vector(1 downto 0);\n";
  F << "		lvds_tx_clk_n : out std_logic_vector(1 downto 0);\n";
  F << "\n";
  F << "		eeprom_scl  : out std_logic;\n";
  F << "		eeprom_sda  : inout std_logic;\n";
  F << "		sys_led_out : out std_logic_vector(5 downto 0);\n";
  F << "		ram_pwr_on  : out std_logic;\n";
  F << "		ram_leds    : out std_logic_vector(1 downto 0);\n";
  F << "		ram_pg      : in std_logic;\n";
  F << "		mgt_pg      : in std_logic\n";
  F << "		);\n";	
  F << "end " << tcn << ";\n";
  F << "\n";
}

void nallacode::writecomp0rx0tx(ostream &F) {
  F << "	component sys_edge_0rx0tx\n";
  F << "		port(\n";
  F << "			clk100p : in STD_LOGIC;\n";
  F << "			clk100n : in STD_LOGIC;\n";
  F << "			reg_clk : in STD_LOGIC;\n";
  F << "			reg_reset_z : in STD_LOGIC;\n";
  F << "			config_data : inout STD_LOGIC_VECTOR(7 downto 0);\n";
  F << "			reg_uds_z : in STD_LOGIC;\n";
  F << "			reg_lds_z : in STD_LOGIC;\n";
  F << "			reg_ads_z : in STD_LOGIC;\n";
  F << "			reg_en_z : in STD_LOGIC;\n";
  F << "			reg_rdy_z : out STD_LOGIC;\n";
  F << "			reg_rd_wr_z : in STD_LOGIC;\n";
  F << "			intra_mod_lvds_comms_control : inout STD_LOGIC_VECTOR(47 downto 0);\n";
  F << "			upper_mod_lvds_comms_control : inout STD_LOGIC_VECTOR(47 downto 0);\n";
  F << "			eeprom_scl : out STD_LOGIC;\n";
  F << "			eeprom_sda : inout STD_LOGIC;\n";
  F << "			sys_led_out : out STD_LOGIC_VECTOR(5 downto 0);\n";
  F << "			ram_leds : out STD_LOGIC_VECTOR(1 downto 0);\n";
  F << "			ram_pwr_on : out STD_LOGIC;\n";
  F << "			ram_pg : in STD_LOGIC;\n";
  F << "			mgt_pg : in STD_LOGIC;\n";
  F << "			osc_clk : out STD_LOGIC;\n";
  F << "			clk200mhz : out STD_LOGIC;\n";
  F << "			clk200mhz_locked : out STD_LOGIC;\n";
  F << "\n";
  F << "			user_reg_clk : out STD_LOGIC;\n";
  F << "			user_interupt : in STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_wdata : out STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_addr : out STD_LOGIC_VECTOR(12 downto 0);\n";
  F << "			user_reg_rden : out STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_wren : out STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_rdy : in STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_rdata0 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_rdata1 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_rdata2 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_rdata3 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "\n";
  F << "			sysmon_alarm : out STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			leds : in STD_LOGIC_VECTOR(3 downto 0));\n";
  F << "	end component;\n";
  F << "\n";
  F << "	signal clk200mhz : std_logic;\n";
  F << "	signal clk200mhz_locked : std_logic;\n";
  F << "        signal clk100 : std_logic;\n";
  F << "\n";
  F << "	signal user_reg_clk : std_logic;\n";
  F << "	signal user_interupt : std_logic_vector(3 downto 0);\n";
  F << "	signal user_reg_wdata : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_addr : std_logic_vector(12 downto 0);\n";
  F << "	signal user_reg_rden : std_logic_vector(3 downto 0);\n";
  F << "	signal user_reg_wren : std_logic_vector(3 downto 0); \n";
  F << "	signal user_reg_rdy : std_logic_vector(3 downto 0);\n";
  F << "	signal user_reg_rdata0 : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_rdata1 : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_rdata2 : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_rdata3 : std_logic_vector(15 downto 0);	 \n";
  F << "\n";
  F << "        signal user_reset : std_logic;\n";
  F << "\n";
  F << "	signal sysmon_alarm : std_logic_vector(3 downto 0);\n";
  F << "	signal leds : std_logic_vector(3 downto 0);\n";
  F << "\n";
  F << "	signal intra_mod_lvds_comms_control:std_logic_vector(47 downto 0);\n";
  F << "	signal upper_mod_lvds_comms_control:std_logic_vector(47 downto 0);\n";
}

void nallacode::writecomp2rx2tx(ostream &F) {  
  F << "	component comp_0_base_edge_2rx2tx\n";
  F << "		port(\n";
  F << "			clk100p : in STD_LOGIC;\n";
  F << "			clk100n : in STD_LOGIC;\n";
  F << "			reg_clk : in STD_LOGIC;\n";
  F << "			reg_reset_z : in STD_LOGIC;\n";
  F << "			config_data : inout STD_LOGIC_VECTOR(7 downto 0);\n";
  F << "			reg_uds_z : in STD_LOGIC;\n";
  F << "			reg_lds_z : in STD_LOGIC;\n";
  F << "			reg_ads_z : in STD_LOGIC;\n";
  F << "			reg_en_z : in STD_LOGIC;\n";
  F << "			reg_rdy_z : out STD_LOGIC;\n";
  F << "			reg_rd_wr_z : in STD_LOGIC;\n";
  F << "\n";
  F << "			lvds_rx_lane_p : in STD_LOGIC_VECTOR(33 downto 0);\n";
  F << "			lvds_rx_lane_n : in STD_LOGIC_VECTOR(33 downto 0);\n";
  F << "			lvds_rx_clk_p : in STD_LOGIC_VECTOR(1 downto 0);\n";
  F << "			lvds_rx_clk_n : in STD_LOGIC_VECTOR(1 downto 0);\n";
  F << "			lvds_tx_lane_p : out STD_LOGIC_VECTOR(33 downto 0);\n";
  F << "			lvds_tx_lane_n : out STD_LOGIC_VECTOR(33 downto 0);\n";
  F << "			lvds_tx_clk_p : out STD_LOGIC_VECTOR(1 downto 0);\n";
  F << "			lvds_tx_clk_n : out STD_LOGIC_VECTOR(1 downto 0);\n";
  F << "\n";
  F << "			intra_mod_lvds_comms_control : inout STD_LOGIC_VECTOR(47 downto 0);\n";
  F << "			upper_mod_lvds_comms_control : inout STD_LOGIC_VECTOR(47 downto 0);\n";
  F << "			eeprom_scl : out STD_LOGIC;\n";
  F << "			eeprom_sda : inout STD_LOGIC;\n";
  F << "			sys_led_out : out STD_LOGIC_VECTOR(5 downto 0);\n";
  F << "			ram_leds : out STD_LOGIC_VECTOR(1 downto 0);\n";
  F << "			ram_pwr_on : out STD_LOGIC;\n";
  F << "			ram_pg : in STD_LOGIC;\n";
  F << "			mgt_pg : in STD_LOGIC;\n";

  F << "			osc_clk : out STD_LOGIC;\n";
  F << "			clk200mhz : out STD_LOGIC;\n";
  F << "			clk200mhz_locked : out STD_LOGIC;\n";
  F << "\n";
  F << "			user_reg_clk : out STD_LOGIC;\n";
  F << "			user_interupt : in STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_wdata : out STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_addr : out STD_LOGIC_VECTOR(12 downto 0);\n";
  F << "			user_reg_rden : out STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_wren : out STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_rdy : in STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			user_reg_rdata0 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_rdata1 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_rdata2 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "			user_reg_rdata3 : in STD_LOGIC_VECTOR(15 downto 0);\n";
  F << "\n";
  F << "			clk : in STD_LOGIC;\n";
  F << "			tx_data_valid : in STD_LOGIC;\n";
  F << "			tx_data : in STD_LOGIC_VECTOR(255 downto 0);\n";
  F << "			tx_data_almost_full : out STD_LOGIC;\n";
  F << "			rx_data_read : in STD_LOGIC;\n";
  F << "			rx_data_empty : out STD_LOGIC;\n";
  F << "			rx_data_valid : out STD_LOGIC;\n";
  F << "			rx_data : out STD_LOGIC_VECTOR(255 downto 0);\n";
  F << "			sysmon_alarm : out STD_LOGIC_VECTOR(3 downto 0);\n";
  F << "			leds : in STD_LOGIC_VECTOR(3 downto 0));\n";
  F << "	end component;\n";
  F << "\n";
  F << "	signal tx_data_valid : std_logic;\n";
  F << "	signal tx_data : std_logic_vector(255 downto 0);\n";
  F << "	signal tx_data_almost_full : std_logic;\n";
  F << "	signal rx_data_valid : std_logic;\n";
  F << "	signal rx_data : std_logic_vector(255 downto 0);\n";
  F << "	signal rx_data_read: std_logic;\n";
  F << "	signal rx_data_empty:std_logic;	 \n";
  F << "\n";
  F << "	signal clk200mhz : std_logic;\n";
  F << "	signal clk200mhz_locked : std_logic;\n";
  F << "	alias  clk : std_logic is clk200mhz;	\n";
  F << "        signal clk100 : std_logic;\n";
  if (clkdiv)
    F << "        signal clk_div2 : std_logic;\n";    
  F << "\n";
  F << "	signal user_reg_clk : std_logic;\n";
  F << "	signal user_interupt : std_logic_vector(3 downto 0);\n";
  F << "	signal user_reg_wdata : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_addr : std_logic_vector(12 downto 0);\n";
  F << "	signal user_reg_rden : std_logic_vector(3 downto 0);\n";
  F << "	signal user_reg_wren : std_logic_vector(3 downto 0); \n";
  F << "	signal user_reg_rdy : std_logic_vector(3 downto 0);\n";
  F << "	signal user_reg_rdata0 : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_rdata1 : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_rdata2 : std_logic_vector(15 downto 0);\n";
  F << "	signal user_reg_rdata3 : std_logic_vector(15 downto 0);	\n"; 
  F << "\n";
  F << "        signal user_reset : std_logic;\n";
  F << "\n";
  F << "	signal sysmon_alarm : std_logic_vector(3 downto 0);\n";
  F << "	signal leds : std_logic_vector(3 downto 0);\n";
  F << "\n";
  F << "	signal intra_mod_lvds_comms_control:std_logic_vector(47 downto 0);\n";
  F << "	signal upper_mod_lvds_comms_control:std_logic_vector(47 downto 0);\n";
}

void nallacode::writeinst0rx0tx(ostream &F) {  
  F << "	EdgeComp : sys_edge_0rx0tx\n";
  F << "	  port map(\n";
  F << "		clk100p          => clk100p,            -- sysio\n";
  F << "		clk100n          => clk100n,            -- sysio\n";
  F << "		reg_clk          => reg_clk,            -- sysio\n";
  F << "		reg_reset_z      => reg_reset_z,        -- sysio\n";
  F << "		config_data      => config_data,        -- sysio\n";
  F << "		reg_uds_z        => reg_uds_z,          -- sysio\n";
  F << "		reg_lds_z        => reg_lds_z,          -- sysio\n";
  F << "		reg_ads_z        => reg_ads_z,          -- sysio\n";
  F << "		reg_en_z         => reg_en_z,           -- sysio\n";
  F << "		reg_rdy_z        => reg_rdy_z,          -- sysio\n";
  F << "		reg_rd_wr_z      => reg_rd_wr_z,        -- sysio\n";
  F << "		intra_mod_lvds_comms_control => intra_mod_lvds_comms_control,\n";
  F << "		upper_mod_lvds_comms_control => upper_mod_lvds_comms_control,\n";
  F << "		eeprom_scl       => eeprom_scl,         -- sysio\n";
  F << "		eeprom_sda       => eeprom_sda,         -- sysio\n";
  F << "		sys_led_out      => sys_led_out,        -- sysio\n";
  F << "		ram_leds         => ram_leds,           -- sysio\n";
  F << "		ram_pwr_on       => ram_pwr_on,         -- sysio\n";
  F << "		ram_pg           => ram_pg,             -- sysio\n";
  F << "		mgt_pg           => mgt_pg,             -- sysio\n";
  F << "		osc_clk          => clk100,             -- gen\n";
  F << "		clk200mhz        => clk200mhz,          -- gen\n";
  F << "		clk200mhz_locked => clk200mhz_locked,   -- gen\n";
  F << "		user_reg_clk     => user_reg_clk,       -- gen\n";
  F << "		user_interupt    => user_interupt,      -- gen\n";
  F << "		user_reg_wdata   => user_reg_wdata,     -- gen\n";
  F << "		user_reg_addr    => user_reg_addr,      -- gen\n";
  F << "		user_reg_rden    => user_reg_rden,      -- gen\n";
  F << "		user_reg_wren    => user_reg_wren,      -- gen\n";
  F << "		user_reg_rdy     => user_reg_rdy,       -- gen\n";
  F << "		user_reg_rdata0  => user_reg_rdata0,    -- gen\n";
  F << "		user_reg_rdata1  => user_reg_rdata1,    -- gen\n";
  F << "		user_reg_rdata2  => user_reg_rdata2,    -- gen\n";
  F << "		user_reg_rdata3  => user_reg_rdata3,    -- gen\n";
  F << "		sysmon_alarm     => sysmon_alarm,       -- gen\n";     
  F << "		leds             => leds                -- gen \n";   
  F << "	);\n";
  F << "	upper_mod_lvds_comms_control(46 downto 30)<=(others=>'0');\n";
  F << "	intra_mod_lvds_comms_control(46 downto 30)<=(others=>'0');\n";
  F << "\n";
}

void nallacode::writeinst2rx2tx(ostream &F) {  
  F << "	comp_0_base_edge_2rx2tx_inst : comp_0_base_edge_2rx2tx\n"; 
  F << "          port map(\n"; 
  F << "	     clk100p          => clk100p,\n"; 
  F << "	     clk100n          => clk100n,\n"; 
  F << "	     reg_clk          => reg_clk,\n"; 
  F << "	     reg_reset_z      => reg_reset_z,\n"; 
  F << "	     config_data      => config_data,\n"; 
  F << "	     reg_uds_z        => reg_uds_z,\n"; 
  F << "	     reg_lds_z        => reg_lds_z,\n"; 
  F << "	     reg_ads_z        => reg_ads_z,\n"; 
  F << "	     reg_en_z         => reg_en_z,\n"; 
  F << "	     reg_rdy_z        => reg_rdy_z,\n"; 
  F << "	     reg_rd_wr_z      => reg_rd_wr_z,\n"; 
  F << "	     lvds_rx_lane_p   => lvds_rx_lane_p,\n"; 
  F << "	     lvds_rx_lane_n   => lvds_rx_lane_n,\n"; 
  F << "	     lvds_rx_clk_p    => lvds_rx_clk_p,\n"; 
  F << "	     lvds_rx_clk_n    => lvds_rx_clk_n,\n"; 
  F << "	     lvds_tx_lane_p   => lvds_tx_lane_p,\n"; 
  F << "	     lvds_tx_lane_n   => lvds_tx_lane_n,\n"; 
  F << "	     lvds_tx_clk_p    => lvds_tx_clk_p,\n"; 
  F << "	     lvds_tx_clk_n    => lvds_tx_clk_n,\n"; 	 
  F << "	     intra_mod_lvds_comms_control=>intra_mod_lvds_comms_control,\n"; 
  F << "	     upper_mod_lvds_comms_control=>upper_mod_lvds_comms_control,\n"; 
  F << "	     eeprom_scl       => eeprom_scl,\n"; 
  F << "	     eeprom_sda       => eeprom_sda,\n"; 
  F << "	     sys_led_out      => sys_led_out,\n"; 
  F << "	     ram_pwr_on       => ram_pwr_on,\n"; 
  F << "	     ram_pg           => ram_pg,\n"; 
  F << "	     mgt_pg           => mgt_pg,  \n"; 
  F << "	     osc_clk          => clk100,\n"; 
  F << "	     clk200mhz        => clk200mhz,\n"; 
  F << "	     clk200mhz_locked => clk200mhz_locked, \n"; 
  F << "	     user_reg_clk     => user_reg_clk,\n"; 
  F << "	     user_interupt    => user_interupt,\n"; 
  F << "	     user_reg_wdata   => user_reg_wdata,\n"; 
  F << "	     user_reg_addr    => user_reg_addr,\n"; 
  F << "	     user_reg_rden    => user_reg_rden,\n"; 
  F << "	     user_reg_wren    => user_reg_wren,	 \n"; 
  F << "	     user_reg_rdy     => user_reg_rdy,\n"; 
  F << "	     user_reg_rdata0  => user_reg_rdata0,\n"; 
  F << "	     user_reg_rdata1  => user_reg_rdata1,\n"; 
  F << "	     user_reg_rdata2  => user_reg_rdata2,\n"; 
  F << "	     user_reg_rdata3  => user_reg_rdata3,\n"; 
  if (clkdiv)
    F << "	       	clk           => clk_div2,\n";
  else
    F << "	       	clk           => clk,\n";
  F << "	     tx_data_valid    => tx_data_valid,\n"; 
  F << "	     tx_data          => tx_data,\n"; 
  F << "	     tx_data_almost_full => tx_data_almost_full,\n"; 
  F << "	     rx_data_valid    => rx_data_valid,	\n"; 
  F << "	     rx_data_read     => rx_data_read,\n"; 
  F << "	     rx_data_empty    => rx_data_empty,\n"; 
  F << "	     rx_data          => rx_data,\n"; 
  F << "	     sysmon_alarm     => sysmon_alarm,\n"; 
  F << "	     leds             => leds,  \n"; 
  F << "	     ram_leds         => ram_leds\n"; 
  F << "	     );\n"; 
  F << "        upper_mod_lvds_comms_control(46 downto 30)<=(others=>'0');\n";
  F << "	intra_mod_lvds_comms_control(46 downto 30)<=(others=>'0');\n";
}

void nallacode::writeucf0rx0tx(ostream &F) {  
  F << "NET \"CONFIG_DATA[0]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AJ26;\n";
  F << "NET \"CONFIG_DATA[1]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK27;\n";
  F << "NET \"CONFIG_DATA[2]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AM14;\n";
  F << "NET \"CONFIG_DATA[3]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN14;\n";
  F << "NET \"CONFIG_DATA[4]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK29;\n";
  F << "NET \"CONFIG_DATA[5]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK28;\n";
  F << "NET \"CONFIG_DATA[6]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP13;\n";
  F << "NET \"CONFIG_DATA[7]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN13;\n";
  F << "NET \"REG_RDY_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AL29;\n";
  F << "NET \"REG_CLK\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK13;\n";
  F << "NET \"REG_ADS_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AL30;\n";
  F << "NET \"REG_RESET_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK15;\n";
  F << "NET \"REG_RD_WR_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK14;\n";
  F << "NET \"REG_UDS_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AJ30;\n";
  F << "NET \"REG_LDS_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK30;\n";
  F << "NET \"REG_EN_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK12;\n";
  F << "NET \"SYS_LED_OUT[0]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN28;\n";
  F << "NET \"SYS_LED_OUT[1]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP27;\n";
  F << "NET \"SYS_LED_OUT[2]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN30;\n";
  F << "NET \"SYS_LED_OUT[3]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP30;\n";
  F << "NET \"SYS_LED_OUT[4]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK17;\n";
  F << "NET \"SYS_LED_OUT[5]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AL17;\n";
  F << "NET \"RAM_PWR_ON\" IOSTANDARD = \"LVCMOS25\" | DRIVE=8 | SLEW=SLOW | LOC=AL14;\n";
  F << "NET \"RAM_LEDS[0]\" IOSTANDARD = \"LVCMOS25\" | DRIVE=8 | SLEW=SLOW | LOC=AN29;\n";
  F << "NET \"RAM_LEDS[1]\" IOSTANDARD = \"LVCMOS25\" | DRIVE=8 | SLEW=SLOW | LOC=AP28;\n";
  F << "NET \"RAM_PG\" IOSTANDARD = \"LVCMOS25\" | LOC=AM29;\n";
  F << "NET \"MGT_PG\" IOSTANDARD = \"LVCMOS25\" | LOC=AM13;\n";
  F << "NET \"EEPROM_SCL\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP16;\n";
  F << "NET \"EEPROM_SDA\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP15;\n";
  F << "NET \"CLK100P\" IOSTANDARD = \"LVDS_25\" | LOC = AL27 | DIFF_TERM = \"TRUE\"; \n";
  F << "NET \"CLK100N\" IOSTANDARD = \"LVDS_25\" | LOC = AL26 | DIFF_TERM = \"TRUE\";\n";
}

void nallacode::writeucf2rx2tx(ostream &F) {  
  F << "NET \"CONFIG_DATA[0]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AJ26;\n";
  F << "NET \"CONFIG_DATA[1]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK27;\n";
  F << "NET \"CONFIG_DATA[2]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AM14;\n";
  F << "NET \"CONFIG_DATA[3]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN14;\n";
  F << "NET \"CONFIG_DATA[4]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK29;\n";
  F << "NET \"CONFIG_DATA[5]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK28;\n";
  F << "NET \"CONFIG_DATA[6]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP13;\n";
  F << "NET \"CONFIG_DATA[7]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN13;\n";
  F << "NET \"REG_RDY_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AL29;\n";
  F << "NET \"REG_CLK\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK13;\n";
  F << "NET \"REG_ADS_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AL30;\n";
  F << "NET \"REG_RESET_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK15;\n";
  F << "NET \"REG_RD_WR_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK14;\n";
  F << "NET \"REG_UDS_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AJ30;\n";
  F << "NET \"REG_LDS_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK30;\n";
  F << "NET \"REG_EN_Z\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK12;\n";
  F << "NET \"SYS_LED_OUT[0]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN28;\n";
  F << "NET \"SYS_LED_OUT[1]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP27;\n";
  F << "NET \"SYS_LED_OUT[2]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AN30;\n";
  F << "NET \"SYS_LED_OUT[3]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP30;\n";
  F << "NET \"SYS_LED_OUT[4]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AK17;\n";
  F << "NET \"SYS_LED_OUT[5]\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AL17;\n";
  F << "NET \"RAM_PWR_ON\" IOSTANDARD = \"LVCMOS25\" | DRIVE=8 | SLEW=SLOW | LOC=AL14;\n";
  F << "NET \"RAM_LEDS[0]\" IOSTANDARD = \"LVCMOS25\" | DRIVE=8 | SLEW=SLOW | LOC=AN29;\n";
  F << "NET \"RAM_LEDS[1]\" IOSTANDARD = \"LVCMOS25\" | DRIVE=8 | SLEW=SLOW | LOC=AP28;\n";
  F << "NET \"RAM_PG\" IOSTANDARD = \"LVCMOS25\" | LOC=AM29;\n";
  F << "NET \"MGT_PG\" IOSTANDARD = \"LVCMOS25\" | LOC=AM13;\n";
  F << "NET \"EEPROM_SCL\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP16;\n";
  F << "NET \"EEPROM_SDA\" IOSTANDARD = \"LVCMOS25\" | DRIVE = 8 | SLEW = SLOW | LOC = AP15;\n";
  F << "NET \"CLK100P\" IOSTANDARD = \"LVDS_25\" | LOC = AL27 | DIFF_TERM = \"TRUE\"; \n";
  F << "NET \"CLK100N\" IOSTANDARD = \"LVDS_25\" | LOC = AL26 | DIFF_TERM = \"TRUE\";\n";
  F << "\n";
  F << "INST \"comp_0_base_edge_2rx2tx_inst/fsb_slave_module_lvds_link_inst/fsb_module_reciever_gen.fsb_module_reciever_inst/single_lane_reciever_gen[1].single_lane_reciever_inst/lvds_phy_rx_inst/RX_IDELAYCTRL\" LOC = \"IDELAYCTRL_X0Y5\" ;\n";
  F << "NET  \"comp_0_base_edge_2rx2tx_inst/fsb_slave_module_lvds_link_inst/fsb_module_reciever_gen.fsb_module_reciever_inst/single_lane_reciever_gen[1].single_lane_reciever_inst/clkdiv\" period=10ns high 50%;\n";
  F << "\n";
  F << "NET \"lvds_rx_lane_p[17]\" IOSTANDARD = \"LVDS_25\" | LOC = AH40 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[17]\" IOSTANDARD = \"LVDS_25\" | LOC = AJ40 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[18]\" IOSTANDARD = \"LVDS_25\" | LOC = AT41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[18]\" IOSTANDARD = \"LVDS_25\" | LOC = AU41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[19]\" IOSTANDARD = \"LVDS_25\" | LOC = AG42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[19]\" IOSTANDARD = \"LVDS_25\" | LOC = AH41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[20]\" IOSTANDARD = \"LVDS_25\" | LOC = AP42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[20]\" IOSTANDARD = \"LVDS_25\" | LOC = AP41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[21]\" IOSTANDARD = \"LVDS_25\" | LOC = AF40 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[21]\" IOSTANDARD = \"LVDS_25\" | LOC = AG41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[22]\" IOSTANDARD = \"LVDS_25\" | LOC = AL42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[22]\" IOSTANDARD = \"LVDS_25\" | LOC = AM42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[23]\" IOSTANDARD = \"LVDS_25\" | LOC = AF41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[23]\" IOSTANDARD = \"LVDS_25\" | LOC = AF42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[24]\" IOSTANDARD = \"LVDS_25\" | LOC = AE40 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[24]\" IOSTANDARD = \"LVDS_25\" | LOC = AD40 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_clk_p[1]\" IOSTANDARD = \"LVDS_25\" | LOC = AB37 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_clk_n[1]\" IOSTANDARD = \"LVDS_25\" | LOC = AB38 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[25]\" IOSTANDARD = \"LVDS_25\" | LOC = AC40 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[25]\" IOSTANDARD = \"LVDS_25\" | LOC = AC39 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[26]\" IOSTANDARD = \"LVDS_25\" | LOC = AC41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[26]\" IOSTANDARD = \"LVDS_25\" | LOC = AD42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[27]\" IOSTANDARD = \"LVDS_25\" | LOC = AE42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[27]\" IOSTANDARD = \"LVDS_25\" | LOC = AD41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[28]\" IOSTANDARD = \"LVDS_25\" | LOC = AB39 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[28]\" IOSTANDARD = \"LVDS_25\" | LOC = AC38 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[29]\" IOSTANDARD = \"LVDS_25\" | LOC = AB41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[29]\" IOSTANDARD = \"LVDS_25\" | LOC = AB42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[30]\" IOSTANDARD = \"LVDS_25\" | LOC = AJ42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[30]\" IOSTANDARD = \"LVDS_25\" | LOC = AJ41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[31]\" IOSTANDARD = \"LVDS_25\" | LOC = AR42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[31]\" IOSTANDARD = \"LVDS_25\" | LOC = AT42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[32]\" IOSTANDARD = \"LVDS_25\" | LOC = AL41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[32]\" IOSTANDARD = \"LVDS_25\" | LOC = AK42 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[33]\" IOSTANDARD = \"LVDS_25\" | LOC = AM41 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[33]\" IOSTANDARD = \"LVDS_25\" | LOC = AN41 | DIFF_TERM=\"TRUE\";\n";
  F << "\n";
  F << "NET \"lvds_tx_lane_p[0]\" IOSTANDARD = \"LVDS_25\" | LOC = AA34;\n";
  F << "NET \"lvds_tx_lane_n[0]\" IOSTANDARD = \"LVDS_25\" | LOC = Y34;\n";
  F << "NET \"lvds_tx_lane_p[1]\" IOSTANDARD = \"LVDS_25\" | LOC = AA35;\n";
  F << "NET \"lvds_tx_lane_n[1]\" IOSTANDARD = \"LVDS_25\" | LOC = AA36;\n";
  F << "NET \"lvds_tx_lane_p[2]\" IOSTANDARD = \"LVDS_25\" | LOC = Y35;\n";
  F << "NET \"lvds_tx_lane_n[2]\" IOSTANDARD = \"LVDS_25\" | LOC = W35;\n";
  F << "NET \"lvds_tx_lane_p[3]\" IOSTANDARD = \"LVDS_25\" | LOC = V39;\n";
  F << "NET \"lvds_tx_lane_n[3]\" IOSTANDARD = \"LVDS_25\" | LOC = W38;\n";
  F << "NET \"lvds_tx_lane_p[4]\" IOSTANDARD = \"LVDS_25\" | LOC = G38;\n";
  F << "NET \"lvds_tx_lane_n[4]\" IOSTANDARD = \"LVDS_25\" | LOC = G39;\n";
  F << "NET \"lvds_tx_lane_p[5]\" IOSTANDARD = \"LVDS_25\" | LOC = E39;\n";
  F << "NET \"lvds_tx_lane_n[5]\" IOSTANDARD = \"LVDS_25\" | LOC = E40;\n";
  F << "NET \"lvds_tx_lane_p[6]\" IOSTANDARD = \"LVDS_25\" | LOC = H38;\n";
  F << "NET \"lvds_tx_lane_n[6]\" IOSTANDARD = \"LVDS_25\" | LOC = H39;\n";
  F << "NET \"lvds_tx_lane_p[7]\" IOSTANDARD = \"LVDS_25\" | LOC = F39;\n";
  F << "NET \"lvds_tx_lane_n[7]\" IOSTANDARD = \"LVDS_25\" | LOC = F40;\n";
  F << "NET \"lvds_tx_clk_p[0]\" IOSTANDARD = \"LVDS_25\" | LOC = T39;\n";
  F << "NET \"lvds_tx_clk_n[0]\" IOSTANDARD = \"LVDS_25\" | LOC = U39;\n";
  F << "NET \"lvds_tx_lane_p[8]\" IOSTANDARD = \"LVDS_25\" | LOC = T37;\n";
  F << "NET \"lvds_tx_lane_n[8]\" IOSTANDARD = \"LVDS_25\" | LOC = U38;\n";
  F << "NET \"lvds_tx_lane_p[9]\" IOSTANDARD = \"LVDS_25\" | LOC = P38;\n";
  F << "NET \"lvds_tx_lane_n[9]\" IOSTANDARD = \"LVDS_25\" | LOC = N38;\n";
  F << "NET \"lvds_tx_lane_p[10]\" IOSTANDARD = \"LVDS_25\" | LOC = R37;\n";
  F << "NET \"lvds_tx_lane_n[10]\" IOSTANDARD = \"LVDS_25\" | LOC = P37;\n";
  F << "NET \"lvds_tx_lane_p[11]\" IOSTANDARD = \"LVDS_25\" | LOC = H40;\n";
  F << "NET \"lvds_tx_lane_n[11]\" IOSTANDARD = \"LVDS_25\" | LOC = J40;\n";
  F << "NET \"lvds_tx_lane_p[12]\" IOSTANDARD = \"LVDS_25\" | LOC = K38;\n";
  F << "NET \"lvds_tx_lane_n[12]\" IOSTANDARD = \"LVDS_25\" | LOC = J38;\n";
  F << "NET \"lvds_tx_lane_p[13]\" IOSTANDARD = \"LVDS_25\" | LOC = K40;\n";
  F << "NET \"lvds_tx_lane_n[13]\" IOSTANDARD = \"LVDS_25\" | LOC = K39;\n";
  F << "NET \"lvds_tx_lane_n[14]\" IOSTANDARD = \"LVDS_25\" | LOC = R38;\n";
  F << "NET \"lvds_tx_lane_p[14]\" IOSTANDARD = \"LVDS_25\" | LOC = R39;\n";
  F << "NET \"lvds_tx_lane_p[15]\" IOSTANDARD = \"LVDS_25\" | LOC = N39;\n";
  F << "NET \"lvds_tx_lane_n[15]\" IOSTANDARD = \"LVDS_25\" | LOC = M39;\n";
  F << "NET \"lvds_tx_lane_p[16]\" IOSTANDARD = \"LVDS_25\" | LOC = M38;\n";
  F << "NET \"lvds_tx_lane_n[16]\" IOSTANDARD = \"LVDS_25\" | LOC = L39;\n";
  F << "\n";
  F << "NET \"lvds_tx_lane_p[17]\" IOSTANDARD = \"LVDS_25\" | LOC = T32;\n";
  F << "NET \"lvds_tx_lane_n[17]\" IOSTANDARD = \"LVDS_25\" | LOC = U32;\n";
  F << "NET \"lvds_tx_lane_p[18]\" IOSTANDARD = \"LVDS_25\" | LOC = P33;\n";
  F << "NET \"lvds_tx_lane_n[18]\" IOSTANDARD = \"LVDS_25\" | LOC = P32;\n";
  F << "NET \"lvds_tx_lane_p[19]\" IOSTANDARD = \"LVDS_25\" | LOC = R33;\n";
  F << "NET \"lvds_tx_lane_n[19]\" IOSTANDARD = \"LVDS_25\" | LOC = R32;\n";
  F << "NET \"lvds_tx_lane_p[20]\" IOSTANDARD = \"LVDS_25\" | LOC = N31;\n";
  F << "NET \"lvds_tx_lane_n[20]\" IOSTANDARD = \"LVDS_25\" | LOC = P31;\n";
  F << "NET \"lvds_tx_lane_p[21]\" IOSTANDARD = \"LVDS_25\" | LOC = K33;\n";
  F << "NET \"lvds_tx_lane_n[21]\" IOSTANDARD = \"LVDS_25\" | LOC = J33;\n";
  F << "NET \"lvds_tx_lane_p[22]\" IOSTANDARD = \"LVDS_25\" | LOC = F31;\n";
  F << "NET \"lvds_tx_lane_n[22]\" IOSTANDARD = \"LVDS_25\" | LOC = F32;\n";
  F << "NET \"lvds_tx_lane_p[23]\" IOSTANDARD = \"LVDS_25\" | LOC = E32;\n";
  F << "NET \"lvds_tx_lane_n[23]\" IOSTANDARD = \"LVDS_25\" | LOC = E33;\n";
  F << "NET \"lvds_tx_lane_p[24]\" IOSTANDARD = \"LVDS_25\" | LOC = G32;\n";
  F << "NET \"lvds_tx_lane_n[24]\" IOSTANDARD = \"LVDS_25\" | LOC = G31;\n";
  F << "NET \"lvds_tx_clk_p[1]\" IOSTANDARD = \"LVDS_25\" | LOC = F35;\n";
  F << "NET \"lvds_tx_clk_n[1]\" IOSTANDARD = \"LVDS_25\" | LOC = E35;\n";
  F << "NET \"lvds_tx_lane_p[25]\" IOSTANDARD = \"LVDS_25\" | LOC = E34;\n";
  F << "NET \"lvds_tx_lane_n[25]\" IOSTANDARD = \"LVDS_25\" | LOC = F34;\n";
  F << "NET \"lvds_tx_lane_p[26]\" IOSTANDARD = \"LVDS_25\" | LOC = M34;\n";
  F << "NET \"lvds_tx_lane_n[26]\" IOSTANDARD = \"LVDS_25\" | LOC = M33;\n";
  F << "NET \"lvds_tx_lane_p[27]\" IOSTANDARD = \"LVDS_25\" | LOC = H34;\n";
  F << "NET \"lvds_tx_lane_n[27]\" IOSTANDARD = \"LVDS_25\" | LOC = G34;\n";
  F << "NET \"lvds_tx_lane_p[28]\" IOSTANDARD = \"LVDS_25\" | LOC = H31;\n";
  F << "NET \"lvds_tx_lane_n[28]\" IOSTANDARD = \"LVDS_25\" | LOC = J31;\n";
  F << "NET \"lvds_tx_lane_p[29]\" IOSTANDARD = \"LVDS_25\" | LOC = G33;\n";
  F << "NET \"lvds_tx_lane_n[29]\" IOSTANDARD = \"LVDS_25\" | LOC = H33;\n";
  F << "NET \"lvds_tx_lane_p[30]\" IOSTANDARD = \"LVDS_25\" | LOC = K32;\n";
  F << "NET \"lvds_tx_lane_n[30]\" IOSTANDARD = \"LVDS_25\" | LOC = J32;\n";
  F << "NET \"lvds_tx_lane_p[31]\" IOSTANDARD = \"LVDS_25\" | LOC = N33;\n";
  F << "NET \"lvds_tx_lane_n[31]\" IOSTANDARD = \"LVDS_25\" | LOC = N34;\n";
  F << "NET \"lvds_tx_lane_p[32]\" IOSTANDARD = \"LVDS_25\" | LOC = M32;\n";
  F << "NET \"lvds_tx_lane_n[32]\" IOSTANDARD = \"LVDS_25\" | LOC = M31;\n";
  F << "NET \"lvds_tx_lane_p[33]\" IOSTANDARD = \"LVDS_25\" | LOC = L32;\n";
  F << "NET \"lvds_tx_lane_n[33]\" IOSTANDARD = \"LVDS_25\" | LOC = L31;\n";
  F << "\n";
  F << "INST \"comp_0_base_edge_2rx2tx_inst/fsb_slave_module_lvds_link_inst/fsb_module_reciever_gen.fsb_module_reciever_inst/single_lane_reciever_gen[0].single_lane_reciever_inst/lvds_phy_rx_inst/RX_IDELAYCTRL\" LOC = \"IDELAYCTRL_X0Y3\" ;\n";
  F << "NET  \"comp_0_base_edge_2rx2tx_inst/fsb_slave_module_lvds_link_inst/fsb_module_reciever_gen.fsb_module_reciever_inst/single_lane_reciever_gen[0].single_lane_reciever_inst/clkdiv\" period=10ns high 50%;\n";
  F << "NET \"lvds_rx_lane_p[0]\" IOSTANDARD = \"LVDS_25\" | LOC = AC33 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[0]\" IOSTANDARD = \"LVDS_25\" | LOC = AD32 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[1]\" IOSTANDARD = \"LVDS_25\" | LOC = AD33 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[1]\" IOSTANDARD = \"LVDS_25\" | LOC = AE32 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[2]\" IOSTANDARD = \"LVDS_25\" | LOC = AE33 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[2]\" IOSTANDARD = \"LVDS_25\" | LOC = AE34 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_clk_p[0]\" IOSTANDARD = \"LVDS_25\" | LOC = AE35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_clk_n[0]\" IOSTANDARD = \"LVDS_25\" | LOC = AF34 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[3]\" IOSTANDARD = \"LVDS_25\" | LOC = AH34 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[3]\" IOSTANDARD = \"LVDS_25\" | LOC = AG34 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[4]\" IOSTANDARD = \"LVDS_25\" | LOC = AL36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[4]\" IOSTANDARD = \"LVDS_25\" | LOC = AL35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[5]\" IOSTANDARD = \"LVDS_25\" | LOC = AN34 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[5]\" IOSTANDARD = \"LVDS_25\" | LOC = AM34 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[6]\" IOSTANDARD = \"LVDS_25\" | LOC = AJ35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[6]\" IOSTANDARD = \"LVDS_25\" | LOC = AK35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[7]\" IOSTANDARD = \"LVDS_25\" | LOC = AB33 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[7]\" IOSTANDARD = \"LVDS_25\" | LOC = AB32 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[8]\" IOSTANDARD = \"LVDS_25\" | LOC = AP35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[8]\" IOSTANDARD = \"LVDS_25\" | LOC = AN36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[9]\" IOSTANDARD = \"LVDS_25\" | LOC = AM36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[9]\" IOSTANDARD = \"LVDS_25\" | LOC = AN35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[10]\" IOSTANDARD = \"LVDS_25\" | LOC = AF35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[10]\" IOSTANDARD = \"LVDS_25\" | LOC = AF36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[11]\" IOSTANDARD = \"LVDS_25\" | LOC = AR37 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[11]\" IOSTANDARD = \"LVDS_25\" | LOC = AT36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[12]\" IOSTANDARD = \"LVDS_25\" | LOC = AT37 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[12]\" IOSTANDARD = \"LVDS_25\" | LOC = AR38 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[13]\" IOSTANDARD = \"LVDS_25\" | LOC = AU38 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[13]\" IOSTANDARD = \"LVDS_25\" | LOC = AU37 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[14]\" IOSTANDARD = \"LVDS_25\" | LOC = AH35 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[14]\" IOSTANDARD = \"LVDS_25\" | LOC = AG36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[15]\" IOSTANDARD = \"LVDS_25\" | LOC = AV39 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[15]\" IOSTANDARD = \"LVDS_25\" | LOC = AV38 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_n[16]\" IOSTANDARD = \"LVDS_25\" | LOC = AJ36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"lvds_rx_lane_p[16]\" IOSTANDARD = \"LVDS_25\" | LOC = AH36 | DIFF_TERM=\"TRUE\";\n";
  F << "NET \"clk100p\" period=10ns high 50%;\n";
  F << "NET \"reg_clk\" period=10ns high 50%;\n";
  F << "NET \"*/clk_rdyn\" TIG;\n";
  F << "NET \"*/module_control_and_status_inst/global_register_inst/lvds_loopback_mode_reg<*>\" TIG;\n";
  F << "NET \"*/module_control_and_status_inst/global_register_inst/fifo_rst_reg<*>\" TIG;\n";
  F << "NET \"*/module_control_and_status_inst/global_register_inst/led_mode_reg<*>\" TIG;\n";
  F << "NET \"*/module_control_and_status_inst/global_register_inst/ram_power_reg<*>\" TIG;\n";
  F << "NET \"*/lvds_link_initialisation_inst/slave_init_sm_gen.lvds_slave_init_sm_inst/link_complete\" TIG;\n";
  F << "NET \"*/fsb_slave_module_lvds_link_inst/lvds_link_initialisation_inst/slave_init_sm_gen.lvds_slave_init_sm_inst/tx_training_done\" TIG;\n";
  F << "# NET \"*/fsb_slave_module_lvds_link_inst/lvds_link_initialisation_inst/master_init_sm_gen.lvds_master_init_sm_inst/tx_training_done\" TIG;\n";
  F << "# NET \"*/fsb_slave_module_lvds_link_inst/lvds_link_initialisation_inst/master_init_sm_gen.lvds_master_init_sm_inst/tx_rst\" TIG;\n";
}

void nallacode::writecompuser(ostream &F, string n) {
  F << "        -- GEZEL-generated user register component\n";
  F << "	component " << n << "\n";
  F << "	   port(\n";
  F << "	       	CLK   : in std_logic; \n";
  F << "	       	RST   : in std_logic; \n";
  F << "		wdata : in std_logic_vector(15 downto 0);\n";
  F << "		addr  : in std_logic_vector(12 downto 0);\n";
  F << "		rden  : in std_logic; \n";
  F << "		wren  : in std_logic; \n";
  F << "		rdata : out std_logic_vector(15 downto 0);\n";
  F << "		rdy   : out std_logic); \n";
  F << "	end component;\n";
  F << "\n";
}

void nallacode::writecompspl(ostream &F, string n) {
  F << "        -- GEZEL-generated SPL component\n";
  F << "	component " << n << "\n";
  F << "	   port(\n";
  F << "	       	CLK   : in std_logic; \n";
  F << "	       	RST   : in std_logic; \n";
  F << "		rx_data:in std_logic_vector(255 downto 0); \n";
  F << "		rx_data_valid:in std_logic;\n";		      
  F << "		rx_data_empty:in std_logic;\n";		      
  F << "		rx_data_read:out std_logic;\n";		      
  F << "		tx_data:out std_logic_vector(255 downto 0);\n";
  F << "		tx_data_valid:out std_logic;\n";		      
  F << "		tx_data_almost_full:in std_logic); \n";         
  F << "	end component;\n";
  F << "\n";
}

void nallacode::writeinstuser(ostream &F, string n, unsigned id) {
  F << "	UserReg" << id << " : " << n << "\n";
  F << "           port map(\n";
  F << "		CLK   => user_reg_clk,\n";
  F << "                RST   => user_reset,\n";
  F << "		wdata => user_reg_wdata,\n";
  F << "		addr  => user_reg_addr,\n";
  F << "		rden  => user_reg_rden(" << id << "),\n";
  F << "		wren  => user_reg_wren(" << id << "),\n";
  F << "		rdata => user_reg_rdata" << id << ",\n";
  F << "		rdy   => user_reg_rdy(" << id << ")\n";
  F << "		);\n";	
}

void nallacode::writeinstspl(ostream &F, string n) {
  F << "	 UserSPL_" << n << " : " << n << "\n";
  F << "	   port map(\n";
  if (clkdiv)
    F << "	       	CLK           => clk_div2,\n";
  else
    F << "	       	CLK           => clk,\n";
  F << "	       	RST           => user_reset,\n";
  F << "		rx_data       => rx_data,\n";
  F << "		rx_data_valid => rx_data_valid,\n";
  F << "		rx_data_empty => rx_data_empty,\n";
  F << "		rx_data_read  => rx_data_read,\n";
  F << "		tx_data       => tx_data,\n";
  F << "		tx_data_valid => tx_data_valid,\n";
  F << "		tx_data_almost_full => tx_data_almost_full\n";
  F << "		);\n";	
}

void nallacode::createtop_fpga0() {
  string topcellname = "fpga0";
  ofstream F("fpga0.vhd");

  switch(edge) {
  case sysedge0rx0tx:
    writetop0rx0tx(F, topcellname);
    break;
  case sysedge2rx2tx:
    writetop2rx2tx(F, topcellname);
    break;
  }

  F << "\n";
  F << "architecture rtl of " << topcellname << " is\n";
  F << "\n";

  switch(edge) {
  case sysedge0rx0tx:
    writecomp0rx0tx(F);
    break;
  case sysedge2rx2tx:
    writecomp2rx2tx(F);
    break;
  }

  // declare spl component
  switch(edge) {
    case sysedge0rx0tx:
    break;
  case sysedge2rx2tx:
    F << "\n";
    writecompspl(F, splcell);
    break;
  }

  // declare each user register component
  map<unsigned, string>::iterator i;

  for (i = userchannel.begin(); i != userchannel.end(); i++) {
    F << "\n";
    writecompuser(F,i->second);
  }

  F << "-- system interconnect\n";
  F << "begin\n";

  // instantiate edge component
  switch(edge) {
  case sysedge0rx0tx:
    writeinst0rx0tx(F);
    break;
  case sysedge2rx2tx:
    writeinst2rx2tx(F);
    break;
  }


  // instantiate spl component
  switch(edge) {
    case sysedge0rx0tx:
    break;
  case sysedge2rx2tx:
    F << "\n";
    writeinstspl(F, splcell);
    break;
  }

  // clkdiv
  if (clkdiv) {
    F << "\n";
    F << "       process(clk)\n";
    F << "       begin\n";
    F << "          if rising_edge(clk) then\n";
    F << "              clk_div2 <= not clk_div2;\n";
    F << "          end if;\n";
    F << "       end process;\n";
  }

  // reset signal for GEZEL component
  F << "\n";
  F << "       process(user_reg_clk)\n";
  F << "       begin\n";
  F << "         if rising_edge(user_reg_clk) then\n";
  F << "            user_reset <= '0';\n";
  F << "            if user_reg_addr = \"0000000000000\" then\n";
  F << "               if user_reg_wren(0) = '1' then\n";
  F << "                  user_reset <= '1';\n";
  F << "               end if;\n";
  F << "            end if;\n";
  F << "         end if;\n";
  F << "       end process;\n";

  // instantiate each user register component
  for (i = userchannel.begin(); i != userchannel.end(); i++) {
    F << "\n";
    writeinstuser(F,i->second,i->first);
  }

  // tie down unused user_reg control signals
  F << "\n";
  unsigned j;
  for (j = 0; j <4; j++) 
    if (userchannel.find(j) == userchannel.end()) {
      if (j == 0)
	F << "        user_reg_rdy(" << j << ") <= user_reg_wren(0);\n";
      else
	F << "        user_reg_rdy(" << j << ") <= '0';\n";
      F << "        user_interupt(" << j << ") <= '0';\n";
      F << "        user_reg_rdata" << j << " <= (others=>'0');\n";
    }
  F << "        leds <= (others=>'0');\n";
  F << "end rtl;\n";
  F.close();

  // write UCF
  ofstream U("fpga0.ucf");
  switch(edge) {
  case sysedge0rx0tx:
    writeucf0rx0tx(U);
    break;
  case sysedge2rx2tx:
    writeucf2rx2tx(U);
    break;
  }
  U.close();

}
