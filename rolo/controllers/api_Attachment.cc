#include "api_Attachment.h"
#include <fstream>
#include <iostream>

using namespace api;
// add definition of your processing function here

void Attachment::download(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::cout << "Entramos\n";
    //auto resp = HttpResponse::newFileResponse("./teste.txt", "", CT_TEXT_PLAIN);
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody("<h1>Murilindo</h1>");
    callback(resp);
}