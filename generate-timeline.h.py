#!/usr/bin/python
import csv
import time
import urllib2

# check http://hackerspaces.org/wiki/Brmlab for the current URL
CSV_URL = "http://hackerspaces.org/wiki/Special:Ask/-5B-5BCategory:Hackerspace-5D-5D-5B-5Bhackerspace-20status::active-5D-5D/-3FCountry/-3FCity/-3FLocation/-3FDate-20of-20founding/-3FWebsite/sort%3DDate_of_founding/order%3DASC/format%3Dcsv/sep%3D,/mainlabel%3Dhackerspace/headers%3Dshow/limit%3D1000"

f = csv.reader(urllib2.urlopen(CSV_URL))
data = []
first = True
for row in f:
    # skip first row
    if first:
        first = False
        continue
    # process location field
    loc = row[3].replace("\xc2\xb0",",").replace("\xe2\x80\xb2",",").replace("\xe2\x80\xb3",",").split(",")
    if len(loc) == 8:
        lat = float(loc[0]) + float(loc[1])/60 + float(loc[2])/3600
        if loc[3] == "S": lat = -lat
        lon = float(loc[4]) + float(loc[5])/60 + float(loc[6])/3600
        if loc[7] == "W": lon = -lon
        row[3] = (lat,lon)
    else:
        row[3] = (0.0,0.0)
    # convert date
    try:
        row[4] = int(time.mktime(time.strptime(row[4], "%d %B %Y")))
    except:
        try:
            row[4] = int(time.mktime(time.strptime(row[4], "%B %Y")))
        except:
            row[4] = int(time.mktime(time.strptime(row[4], "%Y")))
    # append processed data
    data.append(row)

f = open('timeline.h','w')

f.write("struct hsdata {\n")
f.write("    char *name;\n")
f.write("    char *country;\n")
f.write("    char *city;\n")
f.write("    double lat;\n")
f.write("    double lon;\n")
f.write("    time_t date;\n")
f.write("    char *url;\n")
f.write("};\n\n")

f.write("struct hsdata timeline[] = {\n")

for row in data:
    if row[4] < 0:
        continue
    f.write('  { "%s", "%s", "%s", ' % (row[0], row[1], row[2]) )
    f.write('%f, %f, ' % row[3])
    f.write('%d, "%s" },\n' % (row[4], row[5]) )
f.write("  { NULL, NULL, NULL, 0.0, 0.0, 0, NULL }\n");
f.write("};\n")
f.close()
