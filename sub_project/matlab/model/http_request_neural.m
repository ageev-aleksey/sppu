function [u] = http_request_neural(error)
request_data = struct;
request_data.error = error;
body = jsonencode(request_data);
request = matlab.net.http.RequestMessage(matlab.net.http.RequestMethod.POST,...
                                         [],...
                                         body);
response = send(request, '127.0.0.1:8000/neural_compute');
res = jsondecode(response.Body.Data);
u = res.duty_cycle;
end

