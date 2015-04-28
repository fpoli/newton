#!/usr/bin/python

import sys
import logging
import telnetlib
import datetime
import time
import re

#logger = logging.getLogger("")
#logger.setLevel(logging.DEBUG)
#handler = logging.StreamHandler()
#logger.addHandler(handler)
#logger.info("> Connecting to horizons.jpl.nasa.gov:6775 ...")

class Horizons():
	def __init__(self):
		print "> Connecting to horizons.jpl.nasa.gov:6775 ..."
		self.tn = telnetlib.Telnet("horizons.jpl.nasa.gov", 6775)
		#self.tn.set_debuglevel(10)
		self.tn.read_until("Press return to continue -->")
		self.tn.write("\n")
		self.tn.read_until("Horizons>")
		
	def __del__(self):
		print "> Closing connection"
		self.tn.close()
	
	def write(self, command):
		print "> "+command
		self.tn.write(command+"\n")
		time.sleep(1)
		
	def get_planet(self, planet_name, start_date):
		start_date_object = datetime.datetime.strptime(start_date, '%Y-%b-%d %H:%M')
		end_date_object = start_date_object + datetime.timedelta(1) # startdate + 1 day
		end_date = end_date_object.strftime('%Y-%b-%d %H:%M')
		
		self.tn.read_eager() # Clear input buffer
		self.write(planet_name) # Object
		
		obj_info = self.tn.read_until("Select") # Ending of object description
		print obj_info
		
		self.write("e") # Ephemeris
		self.write("v") # Vector
		self.write("@0") # Coordinate center (solar-system barycenter)
		self.write("eclip") # Reference plane (eclipic)
		self.write(start_date) # Starting CT
		self.write(end_date) # Ending   CT
		self.write("1d") # Output interval
		self.write("n") # Accept default output
		self.write("") # Output reference frame
		self.write("1") # Corrections (1=NONE)
		self.write("1") # Output units (1=KM-S)
		self.write("yes") # Spreadsheet CSV format
		self.write("no") # Label cartesian output
		self.write("2") # Output table type ( 2 = State vector {x,y,z,Vx,Vy,Vz} )
		self.write(" ") # Flush input (hack)
		self.write(" ") # Flush input (hack)
		self.write(" ") # Flush input (hack)
		
		self.tn.read_until("$$SOE") # Beginning of Ephemeris
		obj_coord = self.tn.read_until("$$EOE") # Ending of Ephemeris
		print obj_coord
		
		re.split(",", obj_coord)
		
		self.write("n") # Go back to command line, ready for a new case
		self.tn.read_eager()
		
		

horizons = Horizons()
horizons.get_planet("sun", "1997-OCT-15 09:28");

