#
import MySQLdb
from MySQLdb.cursors import DictCursor
from contextlib import closing
from flask import Flask, session, redirect, url_for, abort
from flask import render_template, flash, Blueprint, g
from flask import request, jsonify, Response, json
import json

from api import api

from common import *

@api.route('/hello/datum')
def hi_datum():
    #return jsonify(reply)
    return 'hello datum', 200

#/GetTagInfoByID?TagID=5000010
@api.route('/GetTagInfoByID')
def getTagInfoByID():
    args = request.args
    if ('TagID' not in args):
        return ResponseExt([], 401)

    cur = g.db.cursor(DictCursor)
    # select * from sensorDatum order by rT desc limit 1
    query = 'select * from sensorDatum where TagID="%s" order by rT desc limit 1'%(args['TagID'])
    cur.execute(query)
    rows = cur.fetchall()
    res = [format_datum_row(row) for row in rows]
    cur.close()
    return ResponseExt(res)


#/GetTagInfoByUser?UserID=13501897143
@api.route('/GetTagInfoByUser')
def getTagInfoByUser():
    args = request.args
    if ('UserID' not in args):
        return ResponseExt([], 401)

    cur = g.db.cursor(DictCursor)
    #query = 'select * from sensorDatum where TagID in \
    #        (select TagID from sensors where UserID="%s") \
    #        group by TagID'%(args['UserID'])
    query = 'select * from (select * from sensorDatum ' \
            'where TagID in (select TagID from sensors ' \
            'where UserID=%s) order by rT desc limit 1) ' \
            'as x group by TagID'%(args['UserID'])
    cur.execute(query)
    rows = cur.fetchall()
    res = [format_datum_row(row) for row in rows]
    cur.close()
    return ResponseExt(res)


#/GetTagChartData?TagID=5000002&bTime=2015-01-01%2010:00&eTime=2015-02-27%2011:00&avgTime=60
@api.route('/GetTagChartData')
def getTagChartData():
    args = request.args
    if ('TagID' not in args) \
        or ('bTime' not in args ) \
        or ('eTime' not in args) \
        or ('avgTime' not in args):
        return ResponseExt([], 401)

    try:
        cur = g.db.cursor()
        query = 'select avg(oV), avg(tV), avg(hV), avg(pV), avg(fV), rT from sensorDatum ' \
                'where TagID = "%s" and rT >  "%s" and rT < "%s" ' \
                'group by ROUND(UNIX_TIMESTAMP(rT) / %d) '\
                %(args['TagID'], args['bTime'], args['eTime'], int(args['avgTime']) )
        print query
        cur.execute(query)
        rows = cur.fetchall()
        res = [dict(oValue="%.2f"%(row[0]/100),
                    tValue="%.2f"%(row[1]/10),
                    hValue="%.2f"%(row[2]/10),
                    pValue="%.1f"%(row[3]/100),
                    fValue="%.2f"%(row[4]/1000),
                    Time=row[5].strftime('%H:%M\r\n%m-%d')
                    #Time=str(row[5])
                    ) \
                for row in rows]
        cur.close()
        return ResponseExt(res)
    except Exception as eIns:
        return ResponseExt([], 402)


##
## EOF
