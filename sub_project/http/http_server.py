import http.server 
import json
#from tensorflow import keras

server_address = ("", 8889)
model_path = ""

neural = None

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
		self.wfile.write(b"hello")


if __name__ == "__main__":
	#naural = keras.models.load_model(model_path)
	httpd = http.server.HTTPServer(server_address, HttpHandler)
	print("server-start..")
	httpd.serve_forever()
