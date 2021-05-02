#include "tile-settings02.h"

#define TAG "[tile::settings::settings02]"

static tile_t settings02_tile;
static widget_label_t lbl_title;
static widget_button_t btn_orientation;

void settings_invert_onclick(widget_t *p_widget)
{
  if (twatch_screen_is_inverted())
  {
    /* Disable inverted screen. */
    twatch_screen_set_inverted(false);
    twatch_touch_set_inverted(false);
  }
  else
  {
    /* Enable inverted screen. */
    twatch_screen_set_inverted(true);
    twatch_touch_set_inverted(true);
  }
}

tile_t *tile_settings02_init(void)
{
  /* Initialize our tile. */
  tile_init(&settings02_tile, NULL);

  /* Initialize our title label. */
  widget_label_init(&lbl_title, &settings02_tile, 10, 5, 230, 45, "Orientation");
  widget_set_bg_color(&lbl_title, RGB(0x00,0x66,0x00));
  
  widget_button_init(&btn_orientation, &settings02_tile, 15, 190, 210, 45, "Rotate screen");
  widget_button_set_handler(&btn_orientation, settings_invert_onclick);

  /* Return our tile. */
  return &settings02_tile;
}