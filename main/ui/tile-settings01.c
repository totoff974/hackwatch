#include "tile-settings01.h"

#define TAG "[tile::settings::settings01]"

static tile_t settings01_tile;
static widget_value_select_t hours_select;
static widget_value_select_t mins_select;
static rtc_datetime_t datetime;
static widget_label_t lbl_title;
static widget_button_t btn_save;

/**
 * Custom widget: value selection.
 **/

int _constrain(int min, int max, int value)
{
  if (value < min)
    return min;
  else if (value > max)
    return max;
  return value;
}

void widget_value_select_update_value(widget_value_select_t *p_value_select)
{
  /* Set label text. */
  snprintf(p_value_select->psz_label_text, 3, "%02d", p_value_select->current_value);
  widget_label_set_text(
    &p_value_select->label,
    p_value_select->psz_label_text
  );
}

int widget_value_select_eventhandler(widget_t *p_widget, widget_event_t p_event, int x, int y, int velocity)
{
  widget_value_select_t *p_value_select = (widget_value_select_t *)p_widget;

  switch(p_event)
  {
    case WE_SWIPE_UP:
      p_value_select->current_value = _constrain(p_value_select->min_value, p_value_select->max_value, p_value_select->current_value + 1);
      widget_value_select_update_value(p_value_select);
      return WE_PROCESSED;
      
    case WE_SWIPE_DOWN:
      p_value_select->current_value = _constrain(p_value_select->min_value, p_value_select->max_value, p_value_select->current_value - 1);
      widget_value_select_update_value(p_value_select);
      return WE_PROCESSED;

    default:
      return WE_ERROR;
  }
}

void widget_value_select_init(
  widget_value_select_t *p_value_select,
  tile_t *p_tile,
  int x,
  int y,
  int width,
  int height,
  int min_value,
  int max_value,
  int current_value
)
{
  /* Initialize our label. */
  widget_label_init(&p_value_select->label, p_tile, x, y, width, height, "");
  widget_set_bg_color(&p_value_select->label.widget, RGB(0x3,0x3,0x3));
  widget_set_eventhandler(&p_value_select->label.widget, widget_value_select_eventhandler);

  /* Set min value. */
  p_value_select->min_value = _constrain(0, 99, min_value);

  /* Set max value. */
  p_value_select->max_value = _constrain(p_value_select->min_value, 99, max_value);

  /* Set current value. */
  p_value_select->current_value = _constrain(p_value_select->min_value, p_value_select->max_value, current_value);

  /* Set label text. */
  widget_value_select_update_value(p_value_select);
}

int widget_value_select_get_value(widget_value_select_t *p_value_select)
{
  return p_value_select->current_value;
}

void settings_save_onclick(widget_t *p_widget)
{
  int hours,minutes;

  /* Read hours and minutes. */
  hours = widget_value_select_get_value(&hours_select);
  minutes = widget_value_select_get_value(&mins_select);

  /* Set datetime. */
  twatch_rtc_get_date_time(&datetime);
  datetime.hour = hours;
  datetime.minute = minutes;
  twatch_rtc_set_date_time(&datetime);
}

tile_t *tile_settings01_init(void)
{
  /* Get date and time. */
  twatch_rtc_get_date_time(&datetime);

  /* Initialize our tile. */
  tile_init(&settings01_tile, NULL);

  /* Initialize our title label. */
  widget_label_init(&lbl_title, &settings01_tile, 10, 5, 230, 45, "Settings");
  widget_set_bg_color(&lbl_title, RGB(0x00,0x66,0x00));

  /* Initialize our hours selection widget. */
  widget_value_select_init(&hours_select, &settings01_tile, 10, 55, 60, 80, 0, 23, datetime.hour);

  /* Initialize our hours selection widget. */
  widget_value_select_init(&mins_select, &settings01_tile, 80, 55, 60, 80, 0, 59, datetime.minute);

  /* Initialize our buttons. */
  widget_button_init(&btn_save, &settings01_tile, 15, 140, 210, 45, "Save settings");
  widget_button_set_handler(&btn_save, settings_save_onclick);

  /* Return our tile. */
  return &settings01_tile;
}