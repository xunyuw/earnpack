import os, sys
import json
from flask import request, jsonify, Response, json
from datetime import datetime

import hashlib

def to_sV(strT):
    t = datetime.strptime(strT, '%Y-%m-%d %H:%M:%S')
    diff = (datetime.now() - t).total_seconds()
    return ("0"  if diff > 60 else "1")

def to_md5(txt):
    m = hashlib.md5()
    m.update(txt)
    return m.hexdigest()


def format_datum_row(row):
    return dict(TagID = row["TagID"],
                RSSI=row["RSSI"],
                vList=row["vList"],
                BatInfo="%d%%(%.2fV)"%(row["Bat"]/5, row["Bat"]/100.0),
                oValue="%.2f"%(row["oV"]/100.0),
                tValue="%.1f"%(row["tV"]/10.0),
                hValue="%.1f"%(row["hV"]/10.0),
                pValue="%.1f"%(row["pV"]/100.0),
                fValue="%.2f"%(row["fV"]/1000.0),
                sValue=to_sV(str(row['rT'])),
                rTime=str(row["rT"]),
                nTime=str(row["rT"])
                )

def format_sensor_row(row):
    pass

def format_user_row(row):
    pass


def ResponseExt(jsonObj, st=200):
    return Response(json.dumps(jsonObj),  mimetype='text/html', status=st)

