SET(SOURCES ui_comp_arc1.c
    ui_comp_textarea1.c
    ui_comp.c
    ui_HomePage.c
    ui_CalendarPage.c
    ui_WeatherPage.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_weather_png.c
    ui_img_time_png.c
    ui_img_setting_png.c
    ui_img_qr_png.c
    fonts/ui_font_ChineseSong16.c)

add_library(ui ${SOURCES})
