#
#
import MySQLdb
from MySQLdb.cursors import DictCursor
from contextlib import closing
from flask import Flask, session, redirect, url_for, abort
from flask import render_template, flash, Blueprint, g
from flask import request, jsonify, Response, json
import json
import time

api = Blueprint('api', __name__)
from api import user, sensor, datum

def connect_db():
    dbconnect = MySQLdb.connect(user='root',
                                passwd='xxxxx',
                                db='ehomeDB',
                                host='127.0.0.1')

    return dbconnect


@api.before_request
def before_request():
    g.db = connect_db()


@api.after_request
def after_request(response):
    g.db.close()
    return response

