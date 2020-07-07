//--------------------------------------------------------------
// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
//
// $Id: euclid.java,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $
//--------------------------------------------------------------

class euclid {
    
    public static void main(String[] args) {
	
	System.loadLibrary("gzljava"); // gezel-java interface
	
	GezelModule  G    = new GezelModule ("euclid.fdl");
       	GezelInport  m    = new GezelInport ("m");
       	GezelInport  n    = new GezelInport ("n");
       	GezelInport  ld   = new GezelInport ("ld");
       	GezelOutport gcd  = new GezelOutport("gcd");
       	GezelOutport busy = new GezelOutport("busy");

	n.write(9832);
	m.write(172);
	ld.write(1); // load data

	G.tick();
	G.tick(); 

	while (busy.read() == 1) {
	    //	    System.out.println("tick");
	    G.tick();  // clock until gcd available
	}

	System.out.println("GCD output is " + gcd.read());

    }

}
