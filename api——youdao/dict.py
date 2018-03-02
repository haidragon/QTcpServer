#!/usr/bin/python
#coding: utf-8
#author : ning
#date   : 2013-03-08 21:16:14
import subprocess
import os
import re
import time
import fcntl
import logging

import pygtk
pygtk.require('2.0')
import gtk
import gobject
import webkit
import webbrowser
import youdao_client

VERSION = "0.1.0"
PWD = os.path.dirname(os.path.realpath(__file__))
LOGO = PWD + '/icon.png'

HOME = os.getenv("HOME") + '/.youdao-dict/'
COMMONWORDS_PATH = HOME + '/common_words.txt'
LOG_PATH = HOME + '/dict.log'
LOCK_PATH = HOME +  '/.lock'

if not os.path.exists(HOME):
    os.mkdir(HOME)
if not os.path.exists(COMMONWORDS_PATH):
    file(COMMONWORDS_PATH, 'w').close()

WHITELIST = set( [s.strip() for s in file(COMMONWORDS_PATH).readlines()])

logging.basicConfig(filename=LOG_PATH, level=logging.DEBUG)

class Dict:
    def __init__(self):
        self.mouse_in = False
        self.popuptime = 0
        self.last_selection = ''

        self.window = None
        self.view = None

        self.init_widgets()

    def init_widgets(self):
        '''
        window->vbox->eventbox->view
        '''
        self.window = gtk.Window(gtk.WINDOW_POPUP)
        self.window.set_title("youdao-dict-for-ubuntu")
        self.window.set_border_width(3)
        self.window.connect("destroy", lambda w: gtk.main_quit())
        self.window.resize(360, 200)

        vbox = gtk.VBox(False, 0)
        vbox.show()

        eventbox = gtk.EventBox()
        eventbox.connect('enter-notify-event', self._on_mouse_enter)
        eventbox.connect('leave-notify-event', self._on_mouse_leave)
        gobject.timeout_add(300, self._on_timer, eventbox)
        eventbox.show()

        self.view = webkit.WebView()
        self.view.show()


        #add one by one
        self.window.add(vbox)
        vbox.pack_start(eventbox) # means add
        eventbox.add(self.view)


    def _on_timer(self, widget):
        if self.window.get_property('visible') and not self.mouse_in:
            x, y = self.window.get_position()
            px, py, mods = self.window.get_screen().get_root_window().get_pointer()
            if (px-x)*(px-x) + (py-y)*(py-y) > 500:  # distance > 20 in x, 20 in y
                self.window.hide();
                exit(0)
            if(time.time() - self.popuptime > 5):   # popup for some seconds
                self.window.hide();
                exit(0)
        else:
            if self.mouse_in:
                if(time.time() - self.popuptime > 7):
                    exit(0)
            else :
                if (time.time() - self.popuptime > 5):   # popup for some seconds
                   self.window.hide();
                   exit(0)
            if(time.time() - self.popuptime>25) :
               exit(0)
        return True


    def query_word(self, word):
        js = youdao_client.query(word)
        youdao_client.pronounce(word)
        x, y, mods = self.window.get_screen().get_root_window().get_pointer()
        self.window.move(x+15, y+10)
        self.window.present()
        explains=''
        translation=''
        web =''
        phonetic=''
        if js.has_key('translation'):
            translation = '<br/>'.join(js['translation'])
        if js.has_key('basic'):
             if 'phonetic' in js['basic']:
                 phonetic = js['basic']['phonetic']
             if 'explains' in js['basic']:
                 explains = '<br/>'.join(js['basic']['explains'])
        if js.has_key('web'):
            web = '<br/>'.join( ['<a href="http://dict.youdao.com/search?le=eng&q=%s">%s</a>: %s'%(i['key'],i['key'], ' '.join(i['value'])) for i in js['web'][:3] ] )
        html = '''
<style>
.add_to_wordbook {
    background:  no-repeat;
    vertical-align: middle;
    overflow: hidden;
    display: inline-block;
    vertical-align: top;
    width: 20px;
    padding-top: 20px;
    height: 0;
    margin-left: .5em;
}
</style>
        <h2>
        %(translation)s
        <span style="color: #0B6121; font-size: 13px">< %(phonetic)s > </span>
        </h2>
        <span style="color: #A0A0A0; font-size: 14px"> </span>
        <b>基本翻译:</b>
        <p> %(explains)s </p>
        <span style="color: #A0A0A0; font-size: 14px"> </span>
        <b>网络释意:</b>
        <p> %(web)s </p>
        ''' % locals()
        self.view.load_html_string(html, '')
        self.view.reload()
        self.popuptime = time.time()

    def ignore(self, word):
        if len(word)<=3:
            return True
        if word in WHITELIST:
            return True
        return False

    def _on_mouse_enter(self, wid, event):
        self.popuptime = time.time()
        self.mouse_in = True

    def _on_mouse_leave(self, *args):
        self.mouse_in = False
        self.window.hide()
        exit(0)



def main(words):
    d=Dict()
    d.query_word(words)
    gtk.main()

resWord=""
if __name__ == "__main__":
    f=open(LOCK_PATH, 'w')
    try:
        fcntl.flock(f.fileno(), fcntl.LOCK_EX|fcntl.LOCK_NB)
    except:
        print 'a process is already running!!!'
        exit(0)
    p = subprocess.Popen('xclip -out', shell=True, stdout=subprocess.PIPE)
    out, err = p.communicate()
    for line in out.splitlines():
        resWord+=line
    if not resWord.strip():
        exit(0)
    else:
        main(resWord)
