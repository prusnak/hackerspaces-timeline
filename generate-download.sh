#!/bin/sh

# check http://hackerspaces.org/wiki/Brmlab for the current URL

for i in $(seq 0 10); do
	echo "Downloading ... $i"
	wget --no-check-certificate "https://wiki.hackerspaces.org/Special:Ask/-5B-5BCategory:Hackerspace-5D-5D-5B-5Bhackerspace-20status::active-5D-5D/-3F%3Dhackerspace-23/-3FCountry/-3FCity/-3FLocation/-3FDate-20of-20founding/-3FWebsite/format%3Dcsv/limit%3D100/sort%3DDate_of_founding/mainlabel%3Dhackerspace/offset%3D${i}00" -O download.$i
	tail -n +2 download.$i >> download.data
done
