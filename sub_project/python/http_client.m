request_data = struct;
request_data.begin = 0;
request_data.end = 10;
body = jsonencode(request_data);
request = matlab.net.http.RequestMessage(matlab.net.http.RequestMethod.POST,...
                                         [],...
                                         body);
response = send(request, '127.0.0.1:8888/neural_compute');
result = response.Body.Data;
result