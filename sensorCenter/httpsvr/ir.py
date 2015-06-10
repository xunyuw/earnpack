# -*- coding: utf-8 -*-
"""
"""

from __future__ import with_statement
from contextlib import closing
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash
from flask import Response, json, jsonify
import json
import socket, time


def send2Svr(msg):
    ret = False
    address = ('127.0.0.1', 5012)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if s.connect_ex(address) == 0:
        res = s.send(msg)
        ret = (False if res == 0 else True)
    s.close()
    return ret


ir = Flask(__name__)
ir.config.from_object(__name__)
ir.config.from_envvar('IR_SETTINGS', silent=True)

@ir.route('/hello/ir')
def hi_ir():
    return "hello ir", 200

# http://www.mc-app.net:5013/IR?A0&13501897143&549E7FD&1&0
@ir.route('/IR')
def ir_handler():
    url = request.url.split('?')
    if len(url) != 2:
        return jsonify({"error": "request error"}), 401

    args = url[1].split('&')
    if len(args) != 5:
        return jsonify({"error": "request error"}), 401

    #res = send2svr(url[1])
    msg = url[1]+"&"+"*"*(98-len(url[1]))
    ret = False
    address = ('127.0.0.1', 5012)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if s.connect_ex(address) == 0:
        res = s.send(msg)
        time.sleep(0.3)
        ret = (False if res == 0 else True)
    s.close()

    if ret:
        return jsonify({"result":"OK"}), 200
    else:
        return jsonify({"result":"NOK"}), 500

if __name__ == '__main__':
    ir.run(host='0.0.0.0', port=5013, debug=True)

#
### EOF

