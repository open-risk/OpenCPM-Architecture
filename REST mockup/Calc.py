__author__ = 'oyvind'
#! /usr/bin/env python
"""Creates an OData service """

# SAMPLE_DIR='small-sample'
SAMPLE_DIR = 'Data'


SERVICE_PORT = 8080
SERVICE_ROOT = "http://localhost:%i/" % SERVICE_PORT

import logging
import threading
import time
import string
import os
import StringIO
import os.path
from wsgiref.simple_server import make_server

import pyslet.iso8601 as iso
import pyslet.odata2.csdl as edm
import pyslet.odata2.core as core
import pyslet.odata2.metadata as edmx
from pyslet.odata2.server import ReadOnlyServer
from pyslet.odata2.sqlds import SQLiteEntityContainer
from pyslet.odata2.memds import InMemoryEntityContainer
import pyslet.rfc2616 as http




SAMPLE_DB='calc2.db'

def LoadMetadata(path=os.path.join(os.path.split(__file__)[0],'calcSchema.xml')):
    """Loads the metadata file from the current directory."""
    doc = edmx.Document()
    with open(path, 'rb') as f:
        doc.Read(f)
    return doc

def LoadData(Data, Pairs, dirName):
    for fileName in os.listdir(dirName):
        if fileName[0:2].isdigit() or fileName[-1] == '~':
            # ignore odd files and some editor backups
            continue
        logging.info(
            "Loading data from file %s", os.path.join(dirName, fileName))
        with open(os.path.join(dirName, fileName), 'r') as f:
            LoadDataFromFile(Data,Pairs, f)

def LoadDataFromFile(Data,Pairs, f):
    with Data.OpenCollection() as collection, Pairs.OpenCollection() as pairs_collection:
        while True:
            line = f.readline()
            if len(line) == 0:
                break
            #elif line[1] == '#':
            #    continue
            data = line.split(',')
            if not data:
                continue
            dataPoint = collection.new_entity()
            dataPoint['ID'].set_from_value(data[0])
            dataPoint['Target'].set_from_value(data[1])

            collection.insert_entity(dataPoint)
            # KeyValue Pairs
            for i in xrange((len(data)-3)/2):
                pair=pairs_collection.new_entity()
                pair['ID'].set_from_value(data[0])
                pair['Key'].set_from_value(data[(i+1)*2+1])
                pair['Value'].set_from_value(data[(i+1)*2+2])
                pair['Address'].set_from_value(data[2])
                pairs_collection.InsertEntity(pair)
                dataPoint['KeyValuePairs'].BindEntity(pair)
                collection.update_bindings(dataPoint)


def DryRun():
    doc = LoadMetadata()
    container = InMemoryEntityContainer(
        doc.root.DataServices['CalcSchema.Calc'])
    Data = doc.root.DataServices[
        'CalcSchema.Calc.DataPoints']
    Pairs = doc.root.DataServices[
        'CalcSchema.Calc.KeyValuePairs']
    LoadData(Data, Pairs, SAMPLE_DIR)
    return doc.root.DataServices['CalcSchema.Calc']


def MakeContainer(doc, drop=False, path=SAMPLE_DB):
    if drop and os.path.isfile(path):
        os.remove(path)
    create = not os.path.isfile(path)
    #create=True
    container = SQLiteEntityContainer(
        file_path=path,
        container=doc.root.DataServices['CalcSchema.Calc'])
    if create:
        container.create_all_tables()

def runCalcServer(calcApp=None):
    """Starts the web server running"""
    server = make_server('', SERVICE_PORT, calcApp)
    logging.info("HTTP server on port %i running" % SERVICE_PORT)
    # Respond to requests until process is killed
    server.serve_forever()


def main():
    """Executed when we are launched"""

    doc = LoadMetadata()
    container = MakeContainer(doc,drop=True,path=SAMPLE_DB)
    #container=InMemoryEntityContainer(doc.root.DataServices['CalcSchema.Calc'])
    Data = doc.root.DataServices[
        'CalcSchema.Calc.DataPoints']
    Pairs = doc.root.DataServices[
        'CalcSchema.Calc.KeyValuePairs']
    LoadData(Data, Pairs, SAMPLE_DIR)
    server = ReadOnlyServer(serviceRoot=SERVICE_ROOT)
    server.SetModel(doc)
    t = threading.Thread(
        target=runCalcServer, kwargs={'calcApp': server})
    t.setDaemon(True)
    t.start()
    logging.info("Starting HTTP server on %s" % SERVICE_ROOT)
    t.join()


if __name__ == '__main__':
    #logging.basicConfig(level=logging.INFO)
    main()
