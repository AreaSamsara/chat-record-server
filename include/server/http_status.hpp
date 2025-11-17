#pragma once

namespace AreaSamsara::server
{
    // HTTP状态码常量
    namespace http_status
    {
        // 成功响应
        constexpr int OK = 200;        // 请求成功
        constexpr int Created = 201;   // 资源创建成功
        constexpr int NoContent = 204; // 无内容（操作成功但无需返回数据）

        // 重定向
        constexpr int MovedPermanently = 301; // 资源已永久移动
        constexpr int Found = 302;            // 资源临时移动
        constexpr int NotModified = 304;      // 资源未修改（缓存有效）

        // 客户端错误
        constexpr int BadRequest = 400;          // 请求参数错误
        constexpr int Unauthorized = 401;        // 未认证
        constexpr int Forbidden = 403;           // 无权限访问
        constexpr int NotFound = 404;            // 资源不存在
        constexpr int Conflict = 409;            // 资源冲突（如用户已存在）
        constexpr int UnprocessableEntity = 422; // 请求格式正确但语义错误

        // 服务器错误
        constexpr int InternalServerError = 500; // 服务器内部错误
        constexpr int ServiceUnavailable = 503;  // 服务暂时不可用
    }
}