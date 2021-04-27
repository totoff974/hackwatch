#ifndef __INC_HACKWATCH_CLOCK_H
#define __INC_HACKWATCH_CLOCK_H

#include "twatch.h"
#include "ui/ui.h"
#include "ui/frame.h"

tile_t *tile_clock_init(void);

typedef struct {
  widget_label_t date_lbl;
  /* label date buffer. */
  char psz_label_date[16];
  
} widget_date_t;

#endif /* __INC_HACKWATCH_CLOCK_H */