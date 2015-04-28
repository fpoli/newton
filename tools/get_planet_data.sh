#!/bin/bash

BASENAME="http://ssd.jpl.nasa.gov/horizons.cgi"
BODY="$1"
STARTDATE="$2"
STOPDATE=""
OPT="$3"


if [ "$BODY" = "" ]; then
	BODY="MB:10" #Set Sun as default body
fi

if [ "$STARTDATE" = "" ]; then
	STARTDATE="2010-01-01" #Set Sun as default body
fi

STOPDATE="2018-01-01" #Future occurence


if [ "$OPT" != "keepcookie" ]; then

	#Get cookie (SESSID)
	echo "(*) Retrieving cookies..." 1>&2 
	curl -s -c cookie.txt "$BASENAME" "$BASENAME"  > /dev/null

	#Set "Ephem. type" to "VECTORS"
	echo "(*) Setting: TABLE TYPE" 1>&2 
	curl -s -b cookie.txt -F "table_type=VECTORS" -F "set_table_type=Use Selection Above" -F ".cgifields=table_type" "$BASENAME"  > /dev/null

	#Set table prefs
	echo "(*) Setting: TABLE PREFS" 1>&2 
	curl -s -b cookie.txt -F "output_units=KM-S" -F "vec_quan=2" -F "ref_plane=ECLIPTIC" -F "ref_system=J2000" -F "vect_corr=NONE" -F "csv_format=YES" -F "obj_data=YES" -F "set_table=Use Settings Above" -F "set_table_settings=1" -F ".cgifields=vec_labels" -F ".cgifields=obj_data" -F ".cgifields=csv_format" "$BASENAME"  > /dev/null

	#Set plain text
	echo "(*) Setting: PLAIN TEXT" 1>&2 
	curl -s -b cookie.txt -F "display=TEXT" -F "set_display=Use Selection Above" -F ".cgifields=display" "$BASENAME"  > /dev/null

fi


#Set time prefs
echo "(*) Setting: TIME SPAN [ $STARTDATE ~ $STOPDATE ]" 1>&2 
curl -s -b cookie.txt -F "start_time=$STARTDATE" -F "stop_time=$STOPDATE" -F "step_size=1" -F "interval_mode=Y" -F "set_time_span=Use Specified Times" "$BASENAME"  > /dev/null

echo "(*) Selecting target body [ $BODY ]" 1>&2 
curl -s -b cookie.txt -F "body=$BODY" -F "select_body=Select Indicated Body" "$BASENAME"  > /dev/null

echo "(*) Getting data..." 1>&2 
DATA=$(curl -b cookie.txt -s -o- -F "go=Generate Ephemeris" "$BASENAME")

#echo "$DATA" 1>&2 
echo -e "$DATA" | grep "Target body name" 1>&2 
echo -e "$DATA" | grep "Mass" 1>&2 
echo -e "$DATA" | grep "mass" 1>&2 
echo -e "$DATA" | grep "Radius" 1>&2 
echo -e "$DATA" | grep "radius" 1>&2 

echo -e "$DATA" | grep -A 1 "\$\$SOE" | grep -v "\$\$SOE"

