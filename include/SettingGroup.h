#pragma once

class QString;

namespace setting
{
    enum class SettingGroup
    {
        General, /// < The default group.
        MainWindow, /// < The group regarding the MainWindow class.
    };

    QString to_qstring(const SettingGroup settingGroup);
}
