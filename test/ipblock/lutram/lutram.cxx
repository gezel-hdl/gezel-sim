#include "lutram.h"

extern "C" aipblock *create_lutram(char *instname) {
  return new lutram(instname);
}

lutram::lutram(char *name) : aipblock(name) { 
  wlen = 0;
  alen = 0;
  ramset = 0;
}

void lutram::setparm(char *name) {
  cerr << "lutram: Parameter " << name << " not understood\n";
}

void lutram::run() {
  unsigned idx = ioval[1]->toulong();
  unsigned entries;
  if (ramset) {
    // ram vector initialized
    // cerr << ioval[2]->toulong() << "\n";
    // cerr << ioval[0]->toulong() << "\n";
    *(ioval[3]) = *(content[idx]);
    if (ioval[0]->toulong())
      *(content[idx]) = *(ioval[2]);
  } else {
    // find wordlengh and addr length
    // create a vector of 2^ad entries
    for (entries = 0; 
	 entries < (1U << (ioval[1])->getwordlength()); 
	 entries++)
      content.push_back(make_gval(ioval[2]->getwordlength(), 0));

    //---------- create a VHDL view for this cell
    generate_vhd_view();

    ramset = 1;
  }
}

void lutram::generate_vhd_view() {
  string f(string(name) + ".vhd");
  ofstream V(f.c_str());
  unsigned adlen = ioval[1]->getwordlength();
  unsigned wlen  = ioval[2]->getwordlength();

  V << "library IEEE;			\n";
  V << "use IEEE.STD_LOGIC_1164.ALL;	\n";
  V << "use IEEE.STD_LOGIC_ARITH.ALL;	\n";
  V << "use IEEE.STD_LOGIC_UNSIGNED.ALL;\n";
  V << "\n";
  V << "component " << name << "\n";
  V << "  port (\n";
  V << "   we  :in std_logic;\n";
  V << "   ad  :in std_logic_vector(" << adlen-1 << " downto 0);\n";
  V << "   di  :in std_logic_vector(" << wlen-1 << " downto 0);\n";
  V << "   do  :out std_logic_vector(" << wlen-1 << " downto 0);\n";
  V << "   RST : in std_logic;\n";
  V << "   CLK : in std_logic\n";
  V << ");\n";
  V << "end component;\n";
  V << "\n";
  V << "\n";


  V << "architecture Behavioral of ramexample is\n";
  V << "    type ram_type is array (" << (1 << adlen)-1 << " downto 0)";
  V << " of std_logic_vector (" << wlen-1 << " downto 0);\n";
  V << "    signal RAM : ram_type;\n";
  V << "    begin\n";
  V << "    process (CLK)\n";
  V << "    begin\n";
  V << "        if (CLK'event and CLK = '1') then\n";
  V << "            if (we = '1') then\n";
  V << "                RAM(conv_integer(ad)) <= di;\n";
  V << "            end if;\n";
  V << "        end if;\n";
  V << "    end process;\n";
  V << "    do <= RAM(conv_integer(a));\n";
  V << "end Behavioral;\n";

  V.close();
}

bool lutram::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "we"));
    break;
  case 1:
    return (isinput(d) && isname(tname, "ad"));
    break;
  case 2:
    return (isinput(d) && isname(tname, "di"));
    break;
  case 3:
    return (isoutput(d) && isname(tname, "do"));
    break;
  }
  return false;
}

bool lutram::needsWakeupTest() {
  return false;
}

bool lutram::cannotSleepTest() {
  return false;
}
