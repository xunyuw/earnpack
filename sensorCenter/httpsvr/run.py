# -*- coding: utf-8 -*-
"""
"""

from __future__ import with_statement
import MySQLdb
from MySQLdb.cursors import DictCursor
from contextlib import closing
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash
from flask import Blueprint, jsonify, Response, json
import json

import api

# configuration
DEBUG = True
SECRET_KEY = 'development key'
USERNAME = 'admin'
PASSWORD = 'default'

# create our little app :)
app = Flask(__name__)
app.config.from_object(__name__)
app.config.from_envvar('EHOME_SETTINGS', silent=True)

app.register_blueprint(api.api, url_prefix='/ehome')


@app.route('/')
def show_entries():
	return "welcome", 200

if __name__ == '__main__':
    app.run()
