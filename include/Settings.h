#pragma once

#include "SettingGroup.h"
#include "TypeDisplayer.h"

#include <QSettings>
#include <QString>
#include <QVariant>

#include <cassert>

namespace setting
{

/// @brief Get the setting key.
/// @param group The setting group.
/// @param field The group field.
/// @return A "<group>/<field>" string.
QString get_key(const SettingGroup group, const QString& field);

/// @brief Get the setting key.
/// @param group The setting group.
/// @param field The group field.
/// @return A "<group>/<field>" string.
QString get_key(const QString& group, const QString& field);

template <typename T>
T get(const QString& key, const SettingGroup group, const T& defaultValue)
{
    auto v = QSettings().value(get_key(group, key), defaultValue);
    assert(true == v.canConvert(qMetaTypeId<T>()));
    return qvariant_cast<T>(v);
}

template <typename T>
T get(const QString& key, const SettingGroup group)
{
    auto v = QSettings().value(get_key(group, key));
    assert(true == v.canConvert(qMetaTypeId<T>()));
    return qvariant_cast<T>(v);
}

template <typename T>
void set(const QString& key, const SettingGroup group, const T& value)
{
    auto k = get_key(group, key);
    QSettings().setValue(k, value);
}

}
