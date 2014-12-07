# Requires LibreOffice > 4.3
# Written in python 3.3
# See Webservice.ods for example usage



import pyoo
import subprocess
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
from  urllib.parse import parse_qs
import xml.etree.cElementTree as et

# open the libreoffice background process
subprocess.Popen("soffice \"--accept=socket,host=localhost,port=2002,tcpNoDelay=1;urp;\" --nodefault --nofirststartwizard --nolockcheck --nologo --norestore --invisible", shell=True)
# wait a sec
time.sleep(1)
# connector
desktop = pyoo.Desktop('localhost', 2002)



# build xml from python structures
def data2xml(d, name='data'):
    r = et.Element(name)
    return et.tostring(buildxml(r, d))

def buildxml(r, d):
    if isinstance(d, dict):
        for k, v in d.items():
            s = et.SubElement(r, k)
            buildxml(s, v)
    elif isinstance(d, tuple) or isinstance(d, list):
        for v in d:
            s = et.SubElement(r, 'i')
            buildxml(s, v)
    elif isinstance(d, str):
        r.text = d
    else:
        r.text = str(d)
    return r



# The Libreoffice server - takes a range and a workbook name , returns range
# To do: libreoffice cannot run multiple instance and the first prevents calling the next. Pretty much a showstopper for this idea.

hostName = "localhost"
hostPort = 9000


class MyServer(BaseHTTPRequestHandler):
     def do_OPTIONS(self):
        self.send_response(200, "ok")
        self.send_header('Access-Control-Allow-Credentials', 'true')
        self.send_header('Access-Control-Allow-Origin', 'http://localhost:9000')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header("Access-Control-Allow-Headers", "X-Requested-With, Content-type")

     def do_PROPFIND(self, *args, **kwargs):
        """ just for testing """
        self.send_response(200)

     def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/xml")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        self.wfile.write(bytes("<?xml version=\"1.0\" encoding=\"utf-8\"?>", "utf-8"))
        # I am not an python expert
        query=parse_qs(self.path[2:])
        qr=str(query['path'])[2:-2]
        qr2=str(query['range'])[2:-2]
        ver=qr2.rsplit(sep=',')[0].rsplit(':')
        hor=qr2.rsplit(sep=',')[1].rsplit(':')
        ver_n=[int(ns) for ns in ver]
        hor_n=[int(ns) for ns in hor]
        doc = desktop.open_spreadsheet(qr)
        sheet=doc.sheets[0]
        result=sheet[ver_n[0]:ver_n[1],hor_n[0]:hor_n[1]].values
        self.wfile.write(data2xml(result))
        doc.close();

myServer = HTTPServer((hostName, hostPort), MyServer)
print(time.asctime(), "Server Starts - %s:%s" % (hostName, hostPort))

try:
    myServer.serve_forever()
except KeyboardInterrupt:
    pass

myServer.server_close()
print(time.asctime(), "Server Stops - %s:%s" % (hostName, hostPort))


# stop background proces
subprocess.Popen("pkill \"soffice*\"",shell=True)
#subprocess.Popen("oosplash \"soffice*\"",shell=True)

