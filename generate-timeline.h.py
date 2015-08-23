#!/usr/bin/python
import csv
import time

data = []
f = csv.reader(open('download.data'))
for row in f:
    # process location field
    loc = row[4].replace("\xc2\xb0 ",",").replace("' ",",").replace('"',",").split(",")
    if len(loc) == 8:
        lat = float(loc[0]) + float(loc[1])/60 + float(loc[2])/3600
        if loc[3] == "S": lat = -lat
        lon = float(loc[4]) + float(loc[5])/60 + float(loc[6])/3600
        if loc[7] == "W": lon = -lon
        row[4] = (lat,lon)
    else:
        row[4] = (0.0,0.0)
    # convert date
    try:
        row[5] = int(time.mktime(time.strptime(row[5], "%d %B %Y")))
    except:
        try:
            row[5] = int(time.mktime(time.strptime(row[5], "%B %Y")))
        except:
            row[5] = int(time.mktime(time.strptime(row[5], "%Y")))
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
    f.write('  { "%s", "%s", "%s", ' % (row[0], row[2], row[3]) )
    f.write('%f, %f, ' % row[4])
    f.write('%d, "%s" },\n' % (row[5], row[6]) )
f.write("  { NULL, NULL, NULL, 0.0, 0.0, 0, NULL }\n");
f.write("};\n")
f.close()
