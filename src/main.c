#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_date_layer;
static GFont s_date_font;

static void update_time() {
  //Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  //Create a long lived buffer
  static char buffer [] = "00:00";
  //Adds a date buffer
  static char date_buffer [10];
  //Write the current hours and moniutes into the buffer
  if (clock_is_24h_style () == true){
    //Use a 24 hours format
    strftime (buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else{
    //Use 12 hours
    strftime (buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  strftime(date_buffer, sizeof(date_buffer), "%b %e", tick_time); 
  //Display this time
  text_layer_set_text ( s_time_layer, buffer);
  
 
  //Let it show the date
  text_layer_set_text(s_date_layer, date_buffer);
}

static void main_window_load(Window *window) {
  //Create GBitMap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TEST_PEBBLEFACE);
  s_background_layer = bitmap_layer_create(GRect(0,0,144,168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //Create a date layer
  s_date_layer = text_layer_create (GRect(-5,0, 160, 50));
  text_layer_set_background_color ( s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "7355608");
  //Create a time layer
  s_time_layer = text_layer_create (GRect(20,105,160,50));
  text_layer_set_background_color ( s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  //Create custom font
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_REPORT_40));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_REPORT_35));
  
  //aPPLY TO tEXTLAYER
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //apply to textlayer
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  //Unload time layer
  text_layer_destroy(s_time_layer);
  //Unload date layer
  text_layer_destroy(s_date_layer);
  //Unload GFONT
  fonts_unload_custom_font (s_time_font);
  //Unload GBitmap
  gbitmap_destroy(s_background_bitmap);
  
  // Destroy Bitmaplayer
  bitmap_layer_destroy(s_background_layer);
}
static void tick_handler (struct tm *tick_time, TimeUnits units_changed){
  update_time();
}
static void init() {
  
  //Create static window
  s_main_window = window_create();
  // Set hanglers to manage the elements in the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload 
  });
  //Show window on watch, with animated =true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  //Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}