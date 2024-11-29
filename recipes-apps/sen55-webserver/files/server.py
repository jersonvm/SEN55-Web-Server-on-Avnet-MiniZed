#!/usr/bin/python

import http.server
import socketserver
import logging
import subprocess
import time
import threading

def read_sensor():
	subprocess.run(["./sen55"])

class ServerHandler(http.server.SimpleHTTPRequestHandler):

    def do_GET(self):
        logging.warning("======= GET STARTED =======")
        logging.warning(self.headers)
        http.server.SimpleHTTPRequestHandler.do_GET(self)

PORT = 80

read_sensor_thread = threading.Thread(target=read_sensor)
read_sensor_thread.start()

Handler = ServerHandler
httpd = socketserver.TCPServer(("", PORT), Handler)
httpd.serve_forever()


