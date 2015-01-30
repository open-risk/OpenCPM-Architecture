# Requires LibreOffice > 4.3
# Written in python 3.3
# See Webservice.ods for example usage

import pyoo_deriv
import subprocess
import os
import signal
from http.server import HTTPServer, BaseHTTPRequestHandler
from socketserver import ThreadingMixIn
import threading
import time
import urllib.request
from  urllib.parse import parse_qs, urlparse
import xml.etree.cElementTree as et
import socket
import sqlite3
import os
import os.path
import logging

# open the libreoffice background process - move this onside the request handler., find free port, name tmp file after that port.
# subprocess.Popen("soffice \"--accept=socket,host=localhost,port=2002,tcpNoDelay=1;urp;\" \"-env:UserInstallation=file:///tmp/LibO_Process1\" --nodefault --nofirststartwizard --nolockcheck --nologo --norestore --invisible", shell=True)
# wait a sec
# time.sleep(1)
# connector




# find free port localhost
def get_free_port():
    s = socket.socket()
    s.bind(('', 0))
    port = s.getsockname()[1]
    s.close()
    return port


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


SAMPLE_DIR = 'Data'
SAMPLE_DB = 'calc3.db'


def LoadData(conn, dirName):
    for fileName in os.listdir(dirName):
        if fileName[0:2].isdigit() or fileName[-1] == '~':
            # ignore odd files and some editor backups
            continue
        logging.info(
            "Loading data from file %s", os.path.join(dirName, fileName))
        with open(os.path.join(dirName, fileName), 'r') as f:
            LoadDataFromFile(conn, f)


def LoadDataFromFile(conn, f):
    while True:
        line = f.readline()
        if len(line) == 0:
            break
        # elif line[1] == '#':
        #    continue
        data = line.split(',')
        if not data:
            continue
        cur = conn.cursor()
        cur.execute("INSERT INTO output VALUES(NULL,?,?)", (data[1], data[2]))
        last_id = cur.lastrowid
        # KeyValue Pairs
        for i in range(int((len(data) - 3) / 2)):
            key = 'key'
            value = 'value'
            cur.execute("INSERT INTO keyvalue VALUES(?,?,?)", (last_id, data[(i + 1) * 2 + 1], data[(i + 1) * 2 + 2]))
        conn.commit()
    conn.close()


def MakeContainer(drop=False, path=SAMPLE_DB):
    if drop and os.path.isfile(path):
        os.remove(path)
    create = not os.path.isfile(path)
    # create=True
    conn = sqlite3.connect(path)
    conn.execute('pragma foreign_keys = on')
    cur = conn.cursor()
    if create:
        cur.execute('''CREATE TABLE output (id INTEGER PRIMARY KEY AUTOINCREMENT, target TEXT,address TEXT)''')
        cur.execute(
            '''CREATE TABLE keyvalue (id INTEGER, key TEXT, value TEXT, FOREIGN KEY(id) REFERENCES output(id))''')
    return conn


# The Libreoffice server - takes a range and a workbook name , returns range
# To do: libreoffice cannot run multiple instance and the first prevents calling the next. Pretty much a showstopper for this idea.

hostName = "localhost"
hostPort = 9000

# allocate libreoffice server processes beforehand. Otherwise too slow.
avport = []  # available ports
usedport = []  # used ports
process_ids = []


class Handler(BaseHTTPRequestHandler):
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
        query = parse_qs(self.path[2:])
        path = str(query['path'])[2:-2]
        range = str(query['range'])[2:-2]
        #ver = qr2.rsplit(sep='$')[0].rsplit(':')
        #hor = qr2.rsplit(sep='$')[1].rsplit(':')
        #ver_n = [int(ns) for ns in ver]
        #hor_n = [int(ns) for ns in hor]
        portid = [i for i, val in enumerate(usedport) if val == 0][0]
        port = avport[portid]
        usedport[portid] = 1
        desktop = pyoo_deriv.Desktop('localhost', port)
        doc = desktop.open_spreadsheet(path)
        sheet = doc.sheets[0]
        result = sheet[range].values
        self.wfile.write(data2xml(result))
        doc.close();
        # subprocess.Popen("kill %s" % process,shell=True) #kill process
        usedport[portid] = 0

    def do_QUIT(self):
        """send 200 OK response, and set server.stop to True"""
        self.send_response(200)
        self.end_headers()
        self.server.stop = True
        print(time.asctime(), "Libre Server Stops - %s:%s" % (hostName, hostPort))
        libre_clean()


class MetaHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/xml")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        # self.wfile.write(bytes("<?xml version=\"1.0\" encoding=\"utf-8\"?>", "utf-8"))
        # I am not an python expert
        #params = parse_qs(urlparse(url.strip()).query, keep_blank_values=True)
        query = parse_qs(urlparse(self.path)[4])
        #a=','.join(['(?,?)' for key in query.items()])
        conn = MakeContainer(drop=False, path=SAMPLE_DB)
        cur = conn.cursor()
        cur.execute('''CREATE TEMPORARY TABLE input (key TEXT,value TEXT)''')
        cur.executemany('''INSERT INTO input VALUES (?,?)''', [(k, v[0]) for k, v in query.items()])
        conn.commit()
        sql = '''
        SELECT DISTINCT t.ID,o.address
            FROM keyvalue AS t, output as o
        WHERE NOT EXISTS                            -- this subquery ensures
              ( SELECT *
                    FROM
                input                    -- appear for an object_key
              EXCEPT
                SELECT key, value
                    FROM keyvalue AS tt
                WHERE tt.ID = t.ID
        AND
            o.id=t.id
        )'''
        cur.execute(sql)
        result = cur.fetchall()
        conn.close()
        res= urllib.request.urlopen('http://localhost:9000/?' + result[0][1])
        #cur.execute(sql, (args[0],args[1],args[2],args[3]))
        self.wfile.write(res.read())
    def do_QUIT(self):
        """send 200 OK response, and set server.stop to True"""
        self.send_response(200)
        self.end_headers()
        self.server.stop = True
        print(time.asctime(), "Meta Server Stops - %s:%s" % (hostName, hostPort))

class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    """Handle requests in a separate thread."""


# stop background processes and clean up
def libre_init():
    for x in range(1, 10):
        port = get_free_port()
        str1 = "libreoffice \"-env:UserInstallation=file:///tmp/LibO_Process%s\" --accept=\"socket,port=%s;urp;\" --invisible --nodefault --nofirststartwizard --nolockcheck --nologo --norestore &" % (
            port, port)
        process = subprocess.Popen("exec " + str1, shell=True, preexec_fn=os.setsid)
        avport.append(port)
        usedport.append(0)
        process_ids.append(process)


def libre_clean():
    subprocess.Popen("exec rm /tmp/LibO_Proces* -r", shell=True)
    subprocess.Popen("exec rm /tmp/lu* -r", shell=True)
    subprocess.Popen("exec rm /tmp/OSL_*", shell=True)
    for x in range(0, 9):
        os.killpg(process_ids[x].pid, signal.SIGTERM)


def metaServer():
    conn = MakeContainer(drop=True, path=SAMPLE_DB)
    LoadData(conn, SAMPLE_DIR)
    myMetaServer = ThreadedHTTPServer(('localhost', 9001), MetaHandler)
    print(time.asctime(), "Dispatch Server Started - %s:%s" % (hostName, 9001))
    myMetaServer.serve_forever()


def libreServer():
    libre_init()
    myServer = ThreadedHTTPServer(('localhost', 9000), Handler)
    print(time.asctime(), "Libre Server Started - %s:%s" % (hostName, 9000))
    myServer.serve_forever()


def main():
    try:
        t1 = threading.Thread(target=libreServer)
        t2 = threading.Thread(target=metaServer)
        t1.start()
        #t2.daemon=True
        t2.start()
        while True: time.sleep(100)
    except (KeyboardInterrupt, SystemExit):
        pass
        req = urllib.request.Request("http://localhost:%d" % 9001,method='QUIT')
        f = urllib.request.urlopen(req)
        req = urllib.request.Request("http://localhost:%d" % 9000,method='QUIT')
        f = urllib.request.urlopen(req)


if __name__ == '__main__':
    # logging.basicConfig(level=logging.INFO)
    main()
