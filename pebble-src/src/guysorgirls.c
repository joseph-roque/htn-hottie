#include <pebble.h>
#include "guysorgirls.h"
#include "pop_the_question.h"
#include "utils.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

// Main UI
static Window *s_main_window;
static MenuLayer *s_guy_girl_menu;

// Initialises UI elements
static void initialise_ui(void) {
  s_main_window = window_create();
  #ifdef PBL_COLOR
    window_set_background_color(s_main_window, GColorCeleste);
  #endif
}

// Frees resources from UI elements
static void destroy_ui() {
  window_destroy(s_main_window);
  menu_layer_destroy(s_guy_girl_menu);
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section, void *data) {
  switch (section) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    default:
      return 0;
  }
}

static char* get_header_text(uint16_t section_index) {
  switch (section_index) {
    case 0:
      return "Preferred partner?";
    default:
      return "";
  }
}

static char* get_row_text(uint16_t section, uint16_t row) {
  switch (section) {
    case 0:
      return get_hottie_text_capitalized(row);
    default:
      return "";
  }
}

static char* get_row_subtitle(uint16_t section, uint16_t row) {
  return NULL;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, get_header_text(section));
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  uint16_t section = cell_index->section;
  uint16_t row = cell_index->row;
  menu_cell_basic_draw(ctx, cell_layer, get_row_text(section, row), get_row_subtitle(section, row), NULL);
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  g_guys_girls_both = cell_index->row;
  if (!g_question_shown)
    show_pop_the_question();
  hide_guys_or_girls();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_guy_girl_menu = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_guy_girl_menu, NULL, (MenuLayerCallbacks) {
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(s_guy_girl_menu, window);
  layer_add_child(window_layer, menu_layer_get_layer(s_guy_girl_menu));
}

static void main_window_unload(Window *window) {
  destroy_ui();
}

// Displays this app screen
void show_guys_or_girls(void) {
  initialise_ui();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .unload = main_window_unload,
    .load = main_window_load,
  });
  window_stack_push(s_main_window, true);
}

// Disposes of this app screen
void hide_guys_or_girls(void) {
  window_stack_remove(s_main_window, true);
}
