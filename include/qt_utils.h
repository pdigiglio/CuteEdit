#pragma once

#include <QObject>
#include <QtGlobal>

namespace utils
{

template<typename... Args>
void safe_connect(Args ... args)
{
    auto connection = QObject::connect(std::forward<Args>(args)...);
    if (false == static_cast<bool>(connection))
    {
        qFatal("Connect failed.");
    }
}

}

#define SAFE_CONNECT(...) ::utils::safe_connect(__VA_ARGS__)
