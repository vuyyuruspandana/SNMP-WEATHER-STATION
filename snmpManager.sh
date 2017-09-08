echo -n "enter a number 1 for current data 2 for trend data"
read character
if [ "$character" = "1" ]
then 
	snmpget -v2c -c tempget localhost 1.3.6.1.4.1.777.1.1.0
	snmpget -v2c -c humidityget localhost 1.3.6.1.4.1.777.1.2.0
	snmpget -v2c -c pressget localhost 1.3.6.1.4.1.777.1.3.0
	snmpget -v2c -c locget localhost 1.3.6.1.4.1.777.1.4.0
	snmpget -v2c -c dateget localhost 1.3.6.1.4.1.777.1.5.0
	snmpget -v2c -c windget localhost 1.3.6.1.4.1.777.1.6.0
elif [ "$character" = "2" ]
then 
	snmptable -v2c -c wetget localhost WEATHER-MIB::weatherTable
fi

