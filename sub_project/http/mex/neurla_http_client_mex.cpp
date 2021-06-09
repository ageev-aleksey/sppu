//#include "mex.hpp"
//#include "mexAdapter.hpp"
#include <memory>
#include <vector>

#include <curl/curl.h>
#include <iostream>
#include <sstream>

#include <json/json.h>


#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  neurla_http_client_mex 
#include "simstruc.h"

static double data = 0;
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::string v(ptr, nmemb);
    Json::CharReaderBuilder builder;
    auto reader = builder.newCharReader();
    Json::Value root;
    Json::String err;
    reader->parse(ptr, ptr + nmemb, &root, &err);
    data = root["duty_cycle"].asDouble();
    std::cout << root["duty_cycle"].asDouble() << std::endl;
    std::cout << root << std::endl;
    std::cout << v << std::endl;
}

class MexFunction : public matlab::mex::Function {
public:
    MexFunction() {
        Json::CharReaderBuilder builder;
        m_jsonReader = builder.newCharReader();
        curl_global_init(CURL_GLOBAL_ALL);
        m_curl = curl_easy_init();
        curl_easy_setopt(m_curl, CURLOPT_URL, "http://localhost:8000/");
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_callback);
        m_list = curl_slist_append(m_list, "Content-Type: application/json");
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_list);
        curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");
    }

    ~MexFunction() {
        if (m_list != nullptr) {
            curl_slist_free_all(m_list);
        }
        curl_easy_cleanup(m_curl);
        curl_global_cleanup();
    }
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
        if (inputs.size() == 1 && inputs[0].getType() == matlab::data::ArrayType::DOUBLE) {
            double res = sendPostMessage(inputs[0][0]);
            matlab::data::ArrayFactory factory;
            outputs[0] = factory.createScalar(res);
        } else {
            std::shared_ptr<matlab::engine::MATLABEngine> engine = getEngine();
            matlab::data::ArrayFactory factory;
            engine->feval(u"error", 0,
                          std::vector<matlab::data::Array>({factory.createScalar("must be 1 double argument")}));
        }
    }

    double sendPostMessage(double error) {
        std::stringstream msg;
        msg << "{\"error\": "  << error << "}";
        std::string postData = msg.str();
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postData.c_str());
        CURLcode res = curl_easy_perform(m_curl);
        return data;
    }
private:
    Json::CharReader *m_jsonReader;
    CURL *m_curl;
    struct curl_slist *m_list = nullptr;

};



static DimsInfo_T d;
static void mdlInitializeSizes(SimStruct *S) {
    ssSetNumSFcnParams(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssSetNumInputPorts(S, 1);
    d.width = 1;
    d.numDims = 1;
    d.dims = new int[1];
    d.dims[0] = 1;
    ssSetInputPortDimensionInfo(S, 0, &d);
    ssSetNumOutputPorts(S, 1);
    mdlSetOutputPortDimensionInfo(S, 0, &d);

}


static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1); 
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
}



static void mdlStart(SimStruct *S) {

}


static void mdlOutputs(SimStruct *S, int_T tid) {

}



#ifdef  MATLAB_MEX_FILE    
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif