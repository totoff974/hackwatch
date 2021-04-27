#include "tile-clock.h"
#include "../digits.h"

#define HOURS_X       15
#define HOURS_Y       10
#define MINS_X        120
#define MINS_Y        (240-90-10)
#define DIGIT_WIDTH   56
#define DIGIT_HEIGHT  90

#define TAG "[tile::clock]"


static tile_t clock_tile;
static image_t *clock_digits;
rtc_datetime_t datetime;
int hours=12, mins=34;
widget_date_t date_widget;

int _tile_clock_draw(tile_t *p_tile)
{
  /* Set drawing window. */
  st7789_set_drawing_window(
    p_tile->offset_x,
    p_tile->offset_y,
    p_tile->offset_x + SCREEN_WIDTH,
    p_tile->offset_y + SCREEN_HEIGHT
  );

  /* Fill region with background color. */
  st7789_fill_region(
    p_tile->offset_x,
    p_tile->offset_y,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    p_tile->background_color
  );


  /* Draw digit. */
  tile_bitblt(
    p_tile,
    clock_digits,
    (hours/10)*DIGIT_WIDTH,
    0,
    DIGIT_WIDTH,
    DIGIT_HEIGHT,
    HOURS_X,
    HOURS_Y
  );

  /* Draw digit. */
  tile_bitblt(
    p_tile,
    clock_digits,
    (hours%10)*DIGIT_WIDTH,
    0,
    DIGIT_WIDTH,
    DIGIT_HEIGHT,
    HOURS_X + DIGIT_WIDTH,
    HOURS_Y
  );

  /* Draw digit. */
  tile_bitblt(
    p_tile,
    clock_digits,
    (mins/10)*DIGIT_WIDTH,
    0,
    DIGIT_WIDTH,
    DIGIT_HEIGHT,
    MINS_X,
    MINS_Y
  );

  /* Draw digit. */
  tile_bitblt(
    p_tile,
    clock_digits,
    (mins%10)*DIGIT_WIDTH,
    0,
    DIGIT_WIDTH,
    DIGIT_HEIGHT,
    MINS_X + DIGIT_WIDTH,
    MINS_Y
  );

  /* Draw widgets. */
  tile_draw_widgets(p_tile);

  /* Success. */
  return 0;
}


void clock_update(void *parameter)
{
  while(true)
  {
    twatch_rtc_get_date_time(&datetime);
    hours = datetime.hour;
    mins = datetime.minute;
    snprintf(&date_widget.psz_label_date, 16, "  %02d/%02d/%04d", datetime.day, datetime.month, datetime.year);
    widget_label_set_text(&date_widget.date_lbl, &date_widget.psz_label_date);
    vTaskDelay(300/portTICK_PERIOD_MS);
  }
}


tile_t *tile_clock_init(void)
{
  /* Load digits into memory. */
  clock_digits = load_image(digits_img);

  /* Initialize our tile. */
  tile_init(&clock_tile, NULL);

  /* Add date label */ 
  widget_label_init(&date_widget, &clock_tile, 15, 100, 107, 22, &date_widget.psz_label_date);
  widget_label_set_fontsize(&date_widget, LABEL_FONT_SMALL);
  widget_set_bg_color(&date_widget, RGB(0x00,0x66,0x00));


  /* Set tile drawing function. */
  tile_set_drawfunc(&clock_tile, _tile_clock_draw);

  /* Create update task. */
  xTaskCreate(clock_update, "clock_update", 10000, NULL, 1, NULL);

  /* Return our tile. */
  return &clock_tile;
}