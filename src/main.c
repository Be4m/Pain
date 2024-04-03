#include "application.h"

int main(void)
{
    struct app_settings sett = {
        .win_height = 460,
        .win_width = 900,
        .win_title = "Pain"
    };

    create_application(&sett);
    application_run(&sett);

    return 0;
}