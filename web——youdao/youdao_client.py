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
from bs4 import BeautifulSoup
def json_encode(j):
    return json.dumps(j, indent=4)
def json_decode(j):
    return json.loads(j)

def system(cmd, log=True):
    subprocess.Popen(cmd, shell=True)

def escape(s):
    def _print(x):
        x = x.group()
        code, x = x[0:2], x[2:]
        if code != '\\x':
            return x
        return ('%'+str(x)).upper()

    s = repr(s)
    return re.sub(r'\\x[0-9a-fA-F]{2}', _print, s)
def query_web(word):
    url="http://dict.youdao.com/search?q=%s&keyfrom=new-fanyi.smartResult"%word
    url=url.encode('utf-8')
    resultData=requests.get(url)
    html_doc = resultData.content
    soup = BeautifulSoup(html_doc, "html.parser")
    return soup

def query(word):
    appKey = "yourkey"
    md5Str=appKey+word+"2"+"signkey"
    md5Str=hashlib.md5(md5Str.encode("utf-8")).hexdigest()
    url = 'http://openapi.youdao.com/api?q=%s&from=auto&to=auto&appKey=%s&salt=2&sign=%s'%(word,appKey,md5Str)
    url=url.encode('utf-8')
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
