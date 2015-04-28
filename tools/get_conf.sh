#!/bin/bash

# NasaName RealName Mass Radius Colours r,g,b
#es. PLANET[0]= ("MB:10" "Sole" "100" "100" "5,3,3")
#NASANAME[0]="";	NAME[0]="";	MASS[0]="";	RADIUS[0]=""; COLOR[0]=""; # Default
# Pianeti
NASANAME[${#NASANAME[*]}]="MB:10";	NAME[${#NAME[*]}]="Sole";		MASS[${#MASS[*]}]="1.9891E+30";		RADIUS[${#RADIUS[*]}]="6.960E+8";		COLOR[${#COLOR[*]}]="1, 1, 0"
NASANAME[${#NASANAME[*]}]="MB:1";	NAME[${#NAME[*]}]="Mercurio";	MASS[${#MASS[*]}]="3.302E+23";		RADIUS[${#RADIUS[*]}]="2440E+3";		COLOR[${#COLOR[*]}]="0.8, 0.2, 0"
NASANAME[${#NASANAME[*]}]="MB:2";	NAME[${#NAME[*]}]="Venere";		MASS[${#MASS[*]}]="48.685E+23";		RADIUS[${#RADIUS[*]}]="6051.893E+3";	COLOR[${#COLOR[*]}]="0.8, 0.7, 0.3"
NASANAME[${#NASANAME[*]}]="MB:399";	NAME[${#NAME[*]}]="Terra";		MASS[${#MASS[*]}]="5.9736E+24";		RADIUS[${#RADIUS[*]}]="6371.01E+3";		COLOR[${#COLOR[*]}]="0.1, 0.1, 0.9"

# Luna
NASANAME[${#NASANAME[*]}]="MB:301";	NAME[${#NAME[*]}]="Luna";		MASS[${#MASS[*]}]="734.9E+20";		RADIUS[${#RADIUS[*]}]="1737.53E+3";		COLOR[${#COLOR[*]}]="0.5, 0.5, 0.5"

NASANAME[${#NASANAME[*]}]="MB:4";	NAME[${#NAME[*]}]="Marte";		MASS[${#MASS[*]}]="6.4185E+23";		RADIUS[${#RADIUS[*]}]="3394.0E+3";		COLOR[${#COLOR[*]}]="0.9, 0.1, 0"

# Sonde gemelle Voyager I e II
#NASANAME[${#NASANAME[*]}]="MB:-32";	NAME[${#NAME[*]}]="Voyager II";	MASS[${#MASS[*]}]="721.9";		RADIUS[${#RADIUS[*]}]="7";			COLOR[${#COLOR[*]}]="0.6, 0.2, 0.6"
#NASANAME[${#NASANAME[*]}]="MB:-31";	NAME[${#NAME[*]}]="Voyager I";	MASS[${#MASS[*]}]="721.9";		RADIUS[${#RADIUS[*]}]="7";			COLOR[${#COLOR[*]}]="0.2, 0.6, 0.6"


NASANAME[${#NASANAME[*]}]="MB:5";	NAME[${#NAME[*]}]="Giove";		MASS[${#MASS[*]}]="1898.13E+24";	RADIUS[${#RADIUS[*]}]="69911E+3";		COLOR[${#COLOR[*]}]="0.8, 0.3, 0.1"
NASANAME[${#NASANAME[*]}]="MB:6";	NAME[${#NAME[*]}]="Saturno";	MASS[${#MASS[*]}]="5.68319E+26";	RADIUS[${#RADIUS[*]}]="58232E+3";		COLOR[${#COLOR[*]}]="0.8, 0.6, 0.2"
NASANAME[${#NASANAME[*]}]="MB:7";	NAME[${#NAME[*]}]="Urano";		MASS[${#MASS[*]}]="86.8103E+24";	RADIUS[${#RADIUS[*]}]="25362E+3";		COLOR[${#COLOR[*]}]="0.3, 0.3, 0.9"
NASANAME[${#NASANAME[*]}]="MB:8";	NAME[${#NAME[*]}]="Nettuno";	MASS[${#MASS[*]}]="102.41E+24";		RADIUS[${#RADIUS[*]}]="24624E+3";		COLOR[${#COLOR[*]}]="0.2, 0.2, 0.8"

NASANAME[${#NASANAME[*]}]="MB:9";	NAME[${#NAME[*]}]="Plutone";	MASS[${#MASS[*]}]="1.314E+22";		RADIUS[${#RADIUS[*]}]="1151E+3";		COLOR[${#COLOR[*]}]="0.5, 0.5, 0.6"

# Sonda Cassini - La massa è quella del carburante, quindi dovrebbe diminuire!
#NASANAME[${#NASANAME[*]}]="MB:-82";	NAME[${#NAME[*]}]="Sonda Cassini";	MASS[${#MASS[*]}]="3132";		RADIUS[${#RADIUS[*]}]="6.8";			COLOR[${#COLOR[*]}]="0.5, 0.6, 0.6"

# Satelliti medicei, attorno a Giove
#NASANAME[${#NASANAME[*]}]="MB:501";	NAME[${#NAME[*]}]="Io";			MASS[${#MASS[*]}]="893.3E+20";		RADIUS[${#RADIUS[*]}]="1821.3E+3";		COLOR[${#COLOR[*]}]="0.6, 0.6, 0.7"
#NASANAME[${#NASANAME[*]}]="MB:502";	NAME[${#NAME[*]}]="Europa";		MASS[${#MASS[*]}]="479.7E+20";		RADIUS[${#RADIUS[*]}]="1565E+3";		COLOR[${#COLOR[*]}]="0.6, 0.6, 0.7"
#NASANAME[${#NASANAME[*]}]="MB:503";	NAME[${#NAME[*]}]="Ganymede";	MASS[${#MASS[*]}]="1482E+20";		RADIUS[${#RADIUS[*]}]="2634E+3";		COLOR[${#COLOR[*]}]="0.6, 0.6, 0.7"
#NASANAME[${#NASANAME[*]}]="MB:504";	NAME[${#NAME[*]}]="Callisto";	MASS[${#MASS[*]}]="1076E+20";		RADIUS[${#RADIUS[*]}]="2403E+3";		COLOR[${#COLOR[*]}]="0.6, 0.6, 0.7"

# NasaData ConfName
DATE[${#DATE[*]}]="1997-10-20";		CONFNAME[${#CONFNAME[*]}]="ssol-cassini";
DATE[${#DATE[*]}]="2011-06-08";		CONFNAME[${#CONFNAME[*]}]="ssol-attuale";
#DATE[${#DATE[*]}]="2010-Dec-15";		CONFNAME[${#CONFNAME[*]}]="ssol-2010-Dec-15";
DATE[${#DATE[*]}]="1977-Sep-10";		CONFNAME[${#CONFNAME[*]}]="voyager";
DATE[${#DATE[*]}]="1979-Feb-20";		CONFNAME[${#CONFNAME[*]}]="voyager-Giove";
#DATE[${#DATE[*]}]="2012-06-07";		CONFNAME[${#CONFNAME[*]}]="ssol-attuale_1y5"
#DATE[${#DATE[*]}]="2012-06-08";		CONFNAME[${#CONFNAME[*]}]="ssol-attuale_1y6"
#DATE[${#DATE[*]}]="2012-06-09";		CONFNAME[${#CONFNAME[*]}]="ssol-attuale_1y7"


#Get cookie (SESSID)
echo ""
echo "=== Getting cookie and SESSSID"
bash get_planet_data.sh ${NASANAME[0]} ${DATE[0]}

for (( c=0; c < ${#DATE[*]}; c++ )); do
	
	echo ""
	echo "=== Configuration ${CONFNAME[$c]} ["$((c+1))"/${#DATE[*]}]"
	outfile="${CONFNAME[$c]}.conf"
	
	echo "
// Configuration options

{
	// Ogre interface
	\"camera\" : [0, -4E11, 8E10],
	\"camera_anchor\" : 0,
	\"fps_goal\" : 40,
	\"default_path_anchor\" : 0,
	
	// Simulator memory
	\"path_step\" : 200,
	\"path_length\" : 300,
	
	// Simulation method
	\"method\" : \"rungekutta\",
	\"dt\" : 100,
	\"delete_center_mass_movement\" : 1,
	
	// Planet data
	// http://ssd.jpl.nasa.gov/horizons.cgi
	// Date : ${DATE[$c]}
	
	\"planets\" : [
" > $outfile # overwrite

	first="yes"
	for (( p=0; p < ${#NASANAME[*]}; p++ )); do
		echo ""
		echo "(=) Planet ${NAME[$p]} ["$((p+1))"/${#NASANAME[*]}]"
		outfile="${CONFNAME[$c]}.conf"
		
		data=$(bash get_planet_data.sh ${NASANAME[$p]} ${DATE[$c]} keepcookie)
		
		declare -a coord #declare array 
		coord=( $(echo -e "$data" | sed 's/ //g' | sed 's/,/ /g') )
		
		if [ "$first" = "no" ]; then echo "		," >> $outfile; fi
		first="no"
		
		for (( i=2; i < ${#coord[*]}; i++ )); do
			#echo "(${coord[$i]})*1000" | sed 's/[eE]+/E/g; s/[eE]-/\/10^/g; s/[eE]/*10^/g; s/^/scale=15; /'
			coord[$i]=$(echo "(${coord[$i]})*1000" | sed 's/[eE]+/E/g; s/[eE]-/\/10^/g; s/[eE]/*10^/g; s/^/scale=15; /' | bc | sed 's/^\./0./g; s/^-\./-0./g')
		done
		
		echo "		{" >> $outfile
		echo "			\"name\" : \"${NAME[$p]}\"," >> $outfile
		echo "			\"mass\" : ${MASS[$p]}," >> $outfile
		echo "			\"radius\" : ${RADIUS[$p]}," >> $outfile
		echo "			\"pos\" : [${coord[2]}, ${coord[3]}, ${coord[4]}]," >> $outfile
		echo "			\"vel\" : [${coord[5]}, ${coord[6]}, ${coord[7]}]," >> $outfile
		echo "			\"color\" : [${COLOR[$p]}]" >> $outfile
		echo "		}" >> $outfile
	done
	
	echo "
	]
}
" >> $outfile

done

