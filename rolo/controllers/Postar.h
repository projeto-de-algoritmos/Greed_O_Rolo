#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class Postar : public drogon::HttpSimpleController<Postar>
{
  public:
    virtual void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    PATH_ADD("/postar", Get);
    PATH_LIST_END
};
