#include "Postar.h"
#include <iostream>

void Postar::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto para=req->getParameters();
    HttpViewData data;
    data.insert("title","ListParameters");
    data.insert("parameters",para);
    auto resp=HttpResponse::newHttpViewResponse("Postar.csp",data);
    callback(resp);
}
