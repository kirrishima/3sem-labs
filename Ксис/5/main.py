from random import shuffle

l = [
    ["ipconfig", "/?","/all","/displaydns", "/flushdns", "/renew", "/release", "/registerdns"],
    ["arp", "-a"],
    ["ping", "-t","-a","-n","-l","-w"],
    ["tracert","-d","-h","-w"],
    ["pathping","-n","-h","-q","-w"],
    ["netstat","-a","-n","-o","-r"],
    ["nbtstat","-n","-c","-R","-RR"]
    ]

shuffle(l)

for i in l:
    print()
    print(i[0])
    while len(i) != 1:
        x = input()
        if x in i:
            print("correct")
            i.remove(x)
        else:
            print("Inccorect")