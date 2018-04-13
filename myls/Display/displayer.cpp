#include "Display.h"

using namespace Display;

Displayer::Displayer
        (
            std::ostream &output,
            const Settings::SettingsHolder &settingsHolder,
            const File::FilesSorter &filesSorter
        ):
            output(output),
            settingsHolder(settingsHolder),
            filesSorter(filesSorter)
            {}