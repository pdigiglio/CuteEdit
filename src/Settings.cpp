#include "Settings.h"

#include <QSettings>
#include <QString>
#include <QVariant>


namespace setting
{

namespace
{

QString convert_to_string(SettingGroup group)
{
    return to_qstring(group);
}

QString convert_to_string(const QString& string)
{
    return string;
}

template <typename Group, typename Field>
QString get_key_impl(const Group& group, const Field& field)
{
    return convert_to_string(group) + "/" + convert_to_string(field);
}

}

QString get_key(const SettingGroup group, const QString& field)
{
    return get_key_impl(group, field);
}

QString get_key(const QString& group, const QString& field)
{
    return get_key_impl(group, field);
}

}
