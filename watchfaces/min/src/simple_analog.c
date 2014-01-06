#include "simple_analog.h"

#include "pebble.h"

#include "string.h"
#include "stdlib.h"

Layer *simple_bg_layer;

Layer *date_layer;
TextLayer *day_label;
char day_buffer[6];
TextLayer *num_label;
char num_buffer[4];
TextLayer *battery_layer;

int charge = 5;
bool charging = false;

static GPath *minute_hand;
static GPath *hour_hand;
static GPath *tick_paths[NUM_CLOCK_TICKS];
static GPath *second_bits[NUM_BITS];
static GPath *battery_meter[NUM_BATT];
Layer *hands_layer;
Window *window;

static void handle_battery(BatteryChargeState charge_state) {
  if (charge_state.is_charging) {
    charging = true;
  } else {
    charging = false;
    charge = charge_state.charge_percent/20;
  }
}

static void bg_update_proc(Layer *layer, GContext *ctx) {

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  //graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_draw_outline(ctx, tick_paths[i]);
  }
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  const GPoint center = grect_center_point(&bounds);
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  // minute hand
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  gpath_rotate_to(minute_hand, TRIG_MAX_ANGLE * t->tm_min / 60);
  gpath_draw_filled(ctx, minute_hand);
  gpath_draw_outline(ctx, minute_hand);

  // hour hand
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  
  gpath_rotate_to(hour_hand, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  gpath_draw_filled(ctx, hour_hand);
  gpath_draw_outline(ctx, hour_hand);

  // binary second counter 
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);

  for(int i = 0; i < NUM_BITS; i++) {
    gpath_draw_outline(ctx, second_bits[i]);

    if(t->tm_sec & (1<<i)) {
      gpath_draw_filled(ctx, second_bits[i]);
    }
  }

  // battery meter
  
  for(int i = 0; i < NUM_BATT; i++) {
    gpath_draw_outline(ctx, battery_meter[i]);
    
    if(i < charge) {
      gpath_draw_filled(ctx, battery_meter[i]);
    }
  }
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // init layers
  simple_bg_layer = layer_create(bounds);
  layer_set_update_proc(simple_bg_layer, bg_update_proc);
  layer_add_child(window_layer, simple_bg_layer);

  // init hands
  hands_layer = layer_create(bounds);
  layer_set_update_proc(hands_layer, hands_update_proc);
  layer_add_child(window_layer, hands_layer);
}

static void window_unload(Window *window) {
  layer_destroy(simple_bg_layer);
  layer_destroy(date_layer);
  text_layer_destroy(day_label);
  text_layer_destroy(num_label);
  layer_destroy(hands_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  day_buffer[0] = '\0';
  num_buffer[0] = '\0';

  // init hand paths
  minute_hand = gpath_create(&MINUTE_HAND_POINTS);
  hour_hand = gpath_create(&HOUR_HAND_POINTS);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  const GPoint center = grect_center_point(&bounds);
  gpath_move_to(minute_hand, center);
  gpath_move_to(hour_hand, center);

  // init clock face paths
  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    tick_paths[i] = gpath_create(&ANALOG_BG_POINTS[i]);
  }

  // init second bit paths
  for (int i = 0; i < NUM_BITS; ++i) {
    second_bits[i] = gpath_create(&BINARY_SECONDS[i]);
  }

  // init battery meter paths
  for (int i = 0; i < NUM_BATT; ++i) {
    battery_meter[i] = gpath_create(&BATTERY_METER[i]);
  }

  // Push the window onto the stack
  const bool animated = true;
  window_stack_push(window, animated);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  Layer *root_layer = window_get_root_layer(window);

  GRect frame = layer_get_frame(root_layer);

  battery_layer = text_layer_create(GRect(0,0, /* width */ frame.size.w, 34 /* height */));
  text_layer_set_text_color(battery_layer, GColorWhite);
  text_layer_set_background_color(battery_layer, GColorClear);
  text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(battery_layer, GTextAlignmentLeft);
  text_layer_set_text(battery_layer, "100%");
  //layer_add_child(root_layer, text_layer_get_layer(battery_layer));
  battery_state_service_subscribe(&handle_battery);
  handle_battery(battery_state_service_peek());
}

static void deinit(void) {
  gpath_destroy(minute_hand);
  gpath_destroy(hour_hand);

  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_destroy(tick_paths[i]);
  }
  text_layer_destroy(battery_layer);

  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
