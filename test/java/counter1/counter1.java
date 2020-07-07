//--------------------------------------------------------------
// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
//                                                                             
// $Id: counter1.java,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $
//--------------------------------------------------------------

class counter1 {

    public static void main(String[] args) {

	System.loadLibrary("gzljava"); // gezel-java interface

	GezelModule m = new GezelModule("counter1.fdl");

	for (int i=0; i< 10; i++)
	    m.tick();

    }

}
