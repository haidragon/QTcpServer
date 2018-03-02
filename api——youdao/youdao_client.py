#!/usr/bin/env python


import urllib, urllib2
import os, sys
import re, time
import logging
import tempfile
import commands
import json
import subprocess
import requests
import hashlib
def json_encode(j):
    return json.dumps(j, indent=4)
def json_decode(j):
    return json.loads(j)

def system(cmd, log=True):
    subprocess.Popen(cmd, shell=True)


def query(word):
    #session = requests.Session()
    #url = 'http://fanyi.youdao.com/openapi.do?keyfrom=tinxing&key=1312427901&type=data&doctype=json&version=1.1&q=%s'%(word)
    #response = urllib2.urlopen(url,None)
    #data=response.read()
    #r=requests.get(url)
    #return json_decode(r.text)
    appKey = "ourkey"
    md5Str=''
    md5Source=appKey+word+"2"+"singkey"
    md5Str=hashlib.md5(md5Source.encode("utf-8")).hexdigest()
    urlTemp = 'http://openapi.youdao.com/api?q=%s&from=auto&to=auto&appKey=%s&salt=2&sign=%s'%(word,appKey,md5Str)
    url=urlTemp.encode('utf-8')
    r=requests.get(url)
    return json_decode(r.text)

#this is much simper, 3ks mplayer
def pronounce(word):
    url = 'http://dict.youdao.com/dictvoice?audio=%s'%word
    cmd = 'nohup mplayer "%s" >/dev/null 2>&1 '%(url,)
    system(cmd)

def main(words):
    query(words)
    pronounce(words)
    pass

if __name__ == "__main__":
    p = subprocess.Popen('xclip -out', shell=True, stdout=subprocess.PIPE)
    out, err = p.communicate()
    resWord=""
    for line in out.splitlines():
        resWord+=line
    main(resWord)
    exit(0)
