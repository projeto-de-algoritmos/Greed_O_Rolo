#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace api
{
class Attachment : public drogon::HttpController<Attachment>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    METHOD_ADD(Attachment::download, "/downloadd", Get);
    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void download(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback);
};
} 