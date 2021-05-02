#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "twatch.h"
#include "ui/tile-scanner.h"
#include "ui/tile-apinfo.h"
#include "ui/tile-clock.h"
#include "ui/tile-settings01.h"
#include "ui/tile-settings02.h"
#include "wifi_icon.h"

void main_ui(void *parameter)
{
  tile_t main_tile;
  tile_t wifi_tile;
  tile_t *p_deauth_tile, *p_apinfo_tile, *p_clock_tile, *p_settings01_tile, *p_settings02_tile;
  image_t *wifi;
  
  widget_label_t label_main;
  widget_label_t label_wifi;
  widget_image_t wifi_img;
  widget_label_t wifi_lbl;

  /* Main screen */
  tile_init(&main_tile, NULL);
  widget_label_init(&label_main, &main_tile, 80, 110, 220, 45, "Main tile");

  /* Main screen */
  tile_init(&wifi_tile, NULL);
  wifi = load_image(wifi_icon);
  widget_image_init(&wifi_img, &wifi_tile, 70, (240-88)/2 - 20, 100, 88, wifi);
  widget_label_init(&wifi_lbl, &wifi_tile, 90, 150, 120, 50, "WiFi");

  //widget_label_init(&label_wifi, &wifi_tile, 80, 110, 220, 45, "WiFi tile");



  p_deauth_tile = tile_scanner_init();
  p_apinfo_tile = tile_apinfo_init();
  p_clock_tile = tile_clock_init();
  p_settings01_tile = tile_settings01_init();
  p_settings02_tile = tile_settings02_init();
  
  //tile_link_right(&clock_tile, &wifi_tile);
  tile_link_right(p_clock_tile, &wifi_tile);
  tile_link_left(p_clock_tile, &wifi_tile);

  tile_link_bottom(p_clock_tile, p_settings01_tile);
  tile_link_left(p_settings01_tile, p_settings02_tile);
  tile_link_top(p_settings02_tile, p_clock_tile);

  tile_link_bottom(&wifi_tile, p_deauth_tile);
  tile_link_bottom(p_deauth_tile, p_apinfo_tile);

  /* Select our main tile. */
  ui_select_tile(p_clock_tile);

  while (1)
  {
    ui_process_events();
    vTaskDelay(1);
  }
}

void app_main(void)
{
  //rtc_datetime_t datetime;
  //esp_err_t ret;
  esp_log_level_set("*", ESP_LOG_INFO);

  ui_init();

  /* Check power management. */
  if (twatch_pmu_init() == ESP_OK)
  {
    printf("[pmu] PMU is detected\r\n");
  }
  else
  {
    printf("[pmu] PMU is not detected\r\n");
  }
  /* init rtc before turning display ON */
  twatch_rtc_init();
  twatch_vibrate_init();
  twatch_pmu_init();
  twatch_screen_init();
  /*
  datetime.year = 2021;
  datetime.month=4;
  datetime.day=16;
  datetime.hour=1;
  datetime.minute=00;
  datetime.second=0;
  twatch_rtc_set_date_time(&datetime);
  */

  /* Initialize WiFi controller. */
  wifi_ctrl_init();
  wifi_set_mode(WIFI_SCANNER);

  xTaskCreate(main_ui, "main_ui", 10000, NULL, 1, NULL);
}
