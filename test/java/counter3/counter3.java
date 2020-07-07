//--------------------------------------------------------------
// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
//                                                                             
// $Id: counter3.java,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $
//--------------------------------------------------------------

class counter3 {
    
    public static void main(String[] args) {
	
	System.loadLibrary("gzljava"); // gezel-java interface
	
	GezelModule  m = new GezelModule("counter3.fdl");
       	GezelInport  p = new GezelInport("myinput");

	p.write(5);

	for (int i=0; i< 10; i++) {
	    m.tick();
	}

    }

}
