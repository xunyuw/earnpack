#
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

@api.route('/hello/sensor')
def hi_sensor():
    reply = {"id": "1233", "name":"acad"};
    #return jsonify(reply)
    return 'hello sensor', 200

#/SetUserTagBind?UserID=13501897143&TagID=5000002&Method=1
@api.route('/SetUserTagBind', methods=['GET', 'PUT', 'POST'])
def userTagBind():
    args = request.args
    if ('UserID' not in args) or ("TagID" not in args) or ("Method" not in args):
        return ResponseExt([{"Result":"1"}], 401)

    if (int(args["Method"]) != 0) and (int(args["Method"]) != 1):
        return ResponseExt([{"Result":"1"}], 401)

    cur = g.db.cursor()
    if int(args["Method"]) == 0:
        cur.execute('replace into sensors values(%s, %s)',
                    [args['TagID'].encode('utf-8'),
                     args['UserID'].encode('utf-8')])
    else:
        cur.execute('delete from sensors where TagID = (%s)', [args["TagID"]])

    cur.close()
    g.db.commit()
    return ResponseExt([{"Result":"0"}], 200)

#/GetTagListByUser?UserID=13501897143
@api.route('/GetTagListByUser', methods=['GET'])
def getTagList():
    args = request.args
    if ('UserID' not in args):
        return ResponseExt([], 401)

    cur = g.db.cursor(DictCursor)
    #query = 'select TagID from sensorDatum where TagID in ' \
    #        '(select TagID from sensors where UserID="%s") group by TagID'%(args['UserID'])
    query = 'select TagID from sensors where UserID="%s"'%(args['UserID'])
    #print query
    cur.execute(query)
    rows = cur.fetchall()
    res = [dict(TagID = row["TagID"]) for row in rows]
    cur.close()
    return ResponseExt(res, 200)
