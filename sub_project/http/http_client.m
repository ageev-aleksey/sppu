request_data = struct;
request_data.error = -0.5;
body = jsonencode(request_data);
request = matlab.net.http.RequestMessage(matlab.net.http.RequestMethod.POST,...
                                         [],...
                                         body);
response = send(request, '127.0.0.1:8000/neural_compute');
result = jsondecode(response.Body.Data);
result