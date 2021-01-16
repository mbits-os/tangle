from flask import Flask, request, make_response, redirect, url_for
from pprint import pprint
import sys

app = Flask(__name__)


@app.route('/')
def root():
    resp = make_response('Hello world!')
    resp.headers.set('Content-Type', 'text/plain; charset=UTF-8')
    resp.set_cookie('cookie', value='gaderypoluki', max_age=36000)
    return resp


@app.route('/loop')
def loopA_1():
    return redirect(url_for('loopA_2'))


@app.route('/loop/')
def loopA_2():
    return redirect(url_for('loopA_3'))


@app.route('/loop/third')
def loopA_3():
    return redirect('/loop/fourth')


@app.route('/loop/fourth')
def loopA_4():
    return 'URL: /loop/fourth'


@app.route('/echo')
def echo():
    headers = {}
    for key in request.environ:
        if key[:5] != 'HTTP_':
            continue
        val = request.environ[key]
        key = key[5:].lower().split('_')
        for index in range(len(key)):
            key[index] = key[index][0].upper() + key[index][1:]
        key = '-'.join(key)
        if key not in headers:
            headers[key] = []
        headers[key].append(val)
    text = ''
    for key in sorted(headers.keys()):
        for value in headers[key]:
            text += '{}: {}\n'.format(key, value)
    resp = make_response(text)
    resp.headers.set('Content-Type', 'text/plain; charset=UTF-8')
    return resp


@app.route('/form', methods=['POST'])
def form():
    text = ''
    for key in sorted(request.form.keys()):
        values = request.form.getlist(key)
        for value in values:
            text += 'Field {}: {}\n'.format(key, value)
    if text == '':
        text = '-- EMPTY\n'
    resp = make_response(text)
    resp.headers.set('Content-Type', 'text/plain; charset=UTF-8')
    return resp


@app.route('/upload', methods=['POST'])
def upload():
    text = ''
    data = request.get_json()
    if data is None:
        text = 'Not a JSON'
    else:
        try:
            text = data['value']
        except KeyError:
            text = 'No "value" field'
        text = str(text)
    resp = make_response(text)
    resp.headers.set('Content-Type', 'text/plain; charset=UTF-8')
    return resp
