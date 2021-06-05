MEX_COMPILER=~/app/matlab2020b2/bin/mex
MEX_COMPILER_WIN = "C:/Program Files/MATLAB/R2021a/bin/mex.bat"

mex:
	$(MEX_COMPILER) -g neurla_http_client_sfunc.cpp -L/usr/local/lib/  -lcurl -ljsoncpp

mex_win:
	$(MEX_COMPILER_WIN) COMPFLAGS="$COMPFLAG -I "C:/lib/curl-curl-7_76_1/include" -I "C:/lib/jsoncpp-1.9.4/include" /link /LIBPATH:"C:/lib/curl-curl-7_76_1/build/lib/Release":"C:/lib/jsoncpp-1.9.4/build/lib/Release"" neurla_http_client_sfunc.cpp 