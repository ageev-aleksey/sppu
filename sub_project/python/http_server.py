import http.server 
import json

server_address = ("", 8888)

class HttpHandler (http.server.BaseHTTPRequestHandler):
	def do_GET(self):
		print("request:", self.path)
		self.send_response(200)
		self.send_header('content-type','text/html')
		self.end_headers()
		response = {
			"path": self.path,
			"version": self.protocol_version
		}
		self.wfile.write(json.dumps(response).encode("utf-8"))

	def do_POST(self):
		body_request = self.rfile.read()
		print(body_request)
		self.send_response(200)
		self.end_headers()
		self.wfile.write("")


httpd = http.server.HTTPServer(server_address, HttpHandler)
httpd.serve_forever()
