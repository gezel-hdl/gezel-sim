//--------------------------------------------------------------
// Copyright (C) 2003-2005 P. Schaumont, D. Ching 
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
// $Id: GezelInport.java 21 2005-11-15 19:08:03Z schaum $
//--------------------------------------------------------------

class GezelInport {
    int portId;
    static int glbPortId;    
    public native void portname(String portname);
    public native void write(int n);
    GezelInport(String _portname) {
	portId = glbPortId++;
	portname(_portname);
    }
}

