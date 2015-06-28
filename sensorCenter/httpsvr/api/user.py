# -*- coding: utf-8 -*-
#
import MySQLdb
from MySQLdb.cursors import DictCursor
from contextlib import closing
from flask import Flask, session, redirect, url_for, abort
from flask import render_template, flash, Blueprint, g
from flask import request, jsonify, Response, json
import json

import re

from api import api

from common import *

import httplib
from urllib import urlencode

@api.route('/hello/user')
def hello():
    return 'hello user', 200

#/GetUserExist?UserID=13501897143
@api.route('/GetUserExist', methods=['GET'])
def getUserExist():
    args = request.args
    if ('UserID' not in args):
        return ResponseExt([{"Result":"0"}], 401)

    cur = g.db.cursor(DictCursor)
    query = 'select pwd from users where UserID="%s"'%(args['UserID'])
    cur.execute(query)
    res = cur.fetchall()
    cur.close()
    if len(res) == 0:
        return ResponseExt([{"Result":"0"}], 200)
    else:
        return ResponseExt([{"Result":"1"}], 200)


def sendsms(phone, cnt):
    sp="v.juhe.cn"
    try:
        conn = httplib.HTTPConnection(sp)
        params = urlencode({
            'key':'d8e70a0d4e85d44aa809d32e2308144c',
            'dtype':'json',
            'mobile': phone,
            'tpl_id':'3024',
            'tpl_value':'#app#=空气雷达&#code#=%s'%cnt
            })
        conn.request('GET', '/sms/send?%s'%params)
        print conn.getresponse().read()
        conn.close()
    except Exception as eins:
        print eins


#/GetSmsRandom?UserID=13501897143&Content=123
@api.route('/GetSmsRandom', methods=['GET', 'PUT', 'POST'])
def GetSmsRandom():
    args = request.args
    if ('UserID' not in args) or ('Content' not in args):
        return ResponseExt([{"Result":"1"}], 401)

    code=(re.findall('[0-9]+', args['Content']))[0]
    #sendsms(str(args['UserID']), str(code))
    #threading.Thread(target=sendsms, args=(args['UserID'], str(code))).start()
    phone = str(args['UserID'])
    cnt = str(code)
    sp="v.juhe.cn"
    try:
        conn = httplib.HTTPConnection(sp)
        params = urlencode({
            'key':'d8e70a0d4e85d44aa809d32e2308144c',
            'dtype':'json',
            'mobile': phone,
            'tpl_id':'3024',
            'tpl_value':'#app#=空气雷达&#code#=%s'%cnt
            })
        conn.request('GET', '/sms/send?%s'%params)
        print conn.getresponse().read()
        conn.close()
    except Exception as eins:
        print eins

    return ResponseExt([{"Result":"0"}])
    """
    cur = g.db.cursor(DictCursor)
    query = 'select pwd, name from users where UserID=%s'%(args['UserID'])
    cur.execute(query)
    rows = cur.fetchall()
    res = [dict(pwd=row["pwd"],name=row["name"]) for row in rows ]

    if len(res) > 0:
        cur.execute('replace into users values(%s,%s, %s, %s)',
                [args['UserID'].encode('utf-8'),
                 res[0]['pwd'].encode('utf-8'),
                 res[0]['name'].encode('utf-8'),
                 code.encode('utf-8')])
    else:
        cur.execute('replace into users values(%s, "", "", %s)',
                [args['UserID'].encode('utf-8'),
                 code.encode('utf-8')])

    cur.close()
    g.db.commit()
    #####
    """

#/SetUserRegister?UserID=13482061915&Pwd=123456&Name=jack
@api.route('/SetUserRegister', methods=['GET', 'PUT', 'POST'])
def register():
    args = request.args
    if ('UserID' not in args) or ('Pwd' not in args) or ('Name' not in args):
        return ResponseExt([{"Result":"0"}], 401)

    cur = g.db.cursor(DictCursor)
    query = 'select pwd, name from users where UserID="%s"'%(args['UserID'])
    cur.execute(query)
    rows = cur.fetchall()
    res = [dict(pwd=row["pwd"],name=row["name"]) for row in rows ]

    bExist = (True if len(res) > 0 else False)
    cur.execute('replace into users values(%s, %s, %s, "")',
                [args['UserID'].encode('utf-8'),
                 args['Pwd'].encode('utf-8'),
                 args['Name'].encode('utf-8')])

    cur.close()
    g.db.commit()
    if not bExist:
        return ResponseExt([{"Result":"0"}], 200)
    else:
        return ResponseExt([{"Result":"1"}], 200)

#/GetUserInfo?UserID=13501897143
@api.route('/GetUserInfo', methods=['GET'])
def getUserInfo():
    args = request.args
    if ('UserID' not in args):
        return ResponseExt([{"Name":"null"}], 401)

    cur = g.db.cursor(DictCursor)
    query = 'select name from users where UserID="%s"'%(args['UserID'])
    cur.execute(query)
    rows = cur.fetchall()
    cur.close()
    res = [row["name"] for row in rows]
    if len(res) == 0:
        return ResponseExt([{"Result":"1"}], 200)
    else:
        return ResponseExt([{"Name":res[0]}], 200)

#/SetUserUpdate?UserID=13501897143&Pwd=111111&Name=zhg
@api.route('/SetUserUpdate', methods=['GET', 'PUT', 'POST'])
def SetUserUpdate():
    args = request.args
    if ('UserID' not in args):# or ('Pwd' not in args) or ('Name' not in args):
        return ResponseExt([{"Result":"1"}], 401)

    cur = g.db.cursor(DictCursor)
    query = 'select pwd, name from users where UserID="%s"'%(args['UserID'])
    cur.execute(query)
    rows = cur.fetchall()
    res = [dict(pwd=row["pwd"],name=row["name"]) for row in rows ]

    '''
    if len(res) > 0 \
            and (res[0]['pwd'] != args['Pwd'] \
            or res[0]['name'] != args['Name']):
    '''
    if len(res) > 0:
        pwd = (res[0]['pwd'] if 'Pwd' not in args else args['Pwd'])
        name = (res[0]['name'] if 'Name' not in args else args['Name'])
        cur.execute('update users set pwd=%s, name=%s where UserID=%s' ,
                        [pwd.encode('utf-8'),
                         name.encode('utf-8'),
                         args['UserID']])
    cur.close()
    g.db.commit()

    if len(res) == 0:
        return ResponseExt([{"Result":"1"}], 200)
    else:
        return ResponseExt([{"Result":"0"}], 200)


#/GetLoginVerify?UserID=13501897143&PWD=123456
@api.route('/GetLoginVerify', methods=['GET', 'PUT', 'POST'])
def GetLoginVerify():
    args = request.args
    if ('UserID' not in args) or ('PWD' not in args):
        return ResponseExt([{"Result":"1"}], 401)

    cur = g.db.cursor(DictCursor)
    query = 'select UserID from users where UserID="%s" and pwd="%s"'%(args['UserID'], args['PWD'])
    cur.execute(query)
    rows = cur.fetchall()
    cur.close()
    if len(rows) > 0:
        return ResponseExt([{"Result":"0"}], 200)
    else:
        return ResponseExt([{"Result":"1"}], 200)


#/removeUser?UserID=1351350812
@api.route('/removeUser', methods=['GET', 'PUT', 'POST'])
def removeUser():
    args = request.args
    if 'UserID' not in args:
       return ResponseExt([{"Result":"1"}], 401)

    cur = g.db.cursor()
    query = 'delete from users where UserID="%s"'%(str(args['UserID']))
    cur.execute(query)
    query = 'delete from sensors where UserID="%s"'%(str(args['UserID']))
    cur.execute(query)
    cur.close()
    g.db.commit()
    return ResponseExt([{"Result":"1"}], 401)


#/listUsers
@api.route('/listUsers', methods=['GET'])
def listUsers():
    cur = g.db.cursor(DictCursor)
    query = 'select UserID from users'
    cur.execute(query)
    rows = cur.fetchall()
    cur.close()
    users = [row['UserID'] for row in rows ]
    return ResponseExt(users, 200)



##############################
## IR
#/GetUserIRInfo?UserID=13501897143&KeyID=1
@api.route('/GetUserIRInfo', methods=['GET', 'PUT', 'POST'])
def GetUserIRInfo():
    args = request.args
    if ('UserID' not in args):
        return ResponseExt([], 401)

    cur = g.db.cursor(DictCursor)
    '''
    query="select sms from users where UserID='%s'"%(args['UserID'])
    cur.execute(query)
    row = cur.fetchone()
    cur.close()
    if not row:
        return ResponseExt([], 501)
    print row

    TagID=row["sms"]
    '''

    if ("KeyID" not in args):
        query = 'select KeyID, SeqNo from irInfo where UserID="%s" ' \
                'and pktNum=pktIdx '%(args['UserID'])
    else:
        query = 'select KeyID, SeqNo from irInfo where UserID="%s" ' \
                'and KeyID="%s" and pktNum=pktIdx '%(args['UserID'], args['KeyID'])

    cur.execute(query)
    rows = cur.fetchall()
    cur.close()
    res = [dict(KeyID=str(row["KeyID"]), SeqID=str(row["SeqNo"])) for row in rows ]
    print "=========================="
    print res
    return ResponseExt(res, 200)

##
## EOF
