//--------------------------------------------------------------
// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
//                                                                             
// $Id: hwsw.java,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $
//--------------------------------------------------------------

import avrora.core.Program;
import avrora.sim.InterpreterFactory;
import avrora.sim.platform.Platform;
import avrora.sim.platform.PlatformFactory;
import avrora.sim.mcu.ATMega128L;
import avrora.sim.mcu.Microcontroller;
import avrora.sim.Simulator;
import avrora.util.Terminal;

public class hwsw implements Platform, PlatformFactory {

    protected final Microcontroller mcu;
    protected final Simulator sim;

    protected final HardwareClock hclk;
    protected final GezelModule gzl;

    public hwsw() {
        mcu    = null;
        sim    = null;
	hclk   = null;
        gzl    = null;
	System.out.println("hwsw platform");
    }

    protected class HardwareClock implements Simulator.Event {
	public void fire() {
	    gzl.tick();
        }
    }

    protected class GezelInportPin implements Microcontroller.Pin.Output {
	protected final GezelInport i;
        GezelInportPin(String s) {
	    i = new GezelInport(s);
        }
	public void disableOutput() {}
	public void enableOutput() {}
	public void write(boolean level) {
	    i.write(level ? 1 : 0);
        }
    }

    protected class GezelOutportPin implements Microcontroller.Pin.Input {
	protected final GezelOutport o;
        GezelOutportPin(String s) {
	    o = new GezelOutport(s);
        }
	public void disableInput() {}
	public void enableInput() {}
	public boolean read() {
	    if (o.read() == 0)
		return false;
	    else 
		return true;
        }
    }

    private hwsw(Microcontroller m) {

	hclk = new HardwareClock();
        mcu  = m;
        sim  = m.getSimulator();
	System.loadLibrary("gzljava"); // gezel-java interface
	gzl  = new GezelModule("module.fdl");

        addDevices();
	sim.insertPeriodicEvent(hclk, 1);
    }

    public Microcontroller getMicrocontroller() {
        return mcu;
    }

    public Platform newPlatform(int id, InterpreterFactory f, Program p) {
        return new hwsw(new ATMega128L(false).newMicrocontroller(id, f, p));
    }

    protected void addDevices() {
	mcu.getPin("PA0").connect(new GezelInportPin("PA0"));
	mcu.getPin("PA1").connect(new GezelInportPin("PA1"));
	mcu.getPin("PA2").connect(new GezelOutportPin("PA2"));
	mcu.getPin("PA3").connect(new GezelOutportPin("PA3"));
    }

}
