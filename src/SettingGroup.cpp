#include "SettingGroup.h"

#include <QString>

#include <cassert>

namespace setting
{
    QString to_qstring(const SettingGroup settingGroup)
    {
        switch(settingGroup)
        {
            case SettingGroup::General:
                return "General";

            case SettingGroup::MainWindow:
                return "MainWindow";

            default:
                break;
        }

        assert(false /* Invalid SettingGroup value. */);
        return "";
    }
}
