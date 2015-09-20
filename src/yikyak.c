#include <pebble.h>

static Window *s_menu_window, *s_yakview_window, *s_startup_window, *s_sort_window;
static MenuLayer *s_menu_layer, *s_sort_layer;
static TextLayer *s_yak_text_layer, *s_yakview_text_layer, *s_karma_text_layer, *s_age_text_layer;
static BitmapLayer *s_bitmap_layer;
static GBitmap *s_yak_bitmap;

static char strKarma[4];
static char strAge[5];
static char subyak[15];
static char s_yak_text[32];
static char s_yakview_text[32];
static int newPoints;
static int arrayInd;

typedef struct {
  char yak_text[200];
  int karma;
  int age;
}YakInfo;

typedef struct {
  char sType[5];
}SortType;

static SortType sort_array[] = {
	{"New"},
	{"Hot"},
};

static SortType sort_id;
static YakInfo yak_id;

// static YakInfo *yak_array;

static YakInfo yak_array[] = {
	{"Fuck Harvard", 300, 1},
  {"Prelims got me like (-___________-)", 45, 2},
  {"Microwaves where the food doesn't spin inside creep me the fuck out", 98, 3},
  {"BravoTa.co is awesome, go and check it out", 5, 3},
  {"Hackathon is the coolest place to meet friends", 122, 5},
  {"HMU Study Buddy 4 life, will study buddy for tuition", 22, 6},
  {"Assert dominance by telling the professor YOUR office hours" , 72, 8},
  {"I heart emoticon my pebble!!", 35, 10},
  {"Cornell is the best college ever upvote if you agree", 65, 14},
  {"Im doin buddy study for my project, ", 88, 17},
};

// ___________________________________________________________________________________________________________
// SORT
// ___________________________________________________________________________________________________________

static void sortAge(){     
	int n = sizeof(yak_array) / sizeof(YakInfo);
  YakInfo temp;
  for(int i=1;i<n;i++) {
    for(int j=0;j<n-i;j++) {
      if(yak_array[j].age >yak_array[j+1].age) {
        temp=yak_array[j];
        yak_array[j]=yak_array[j+1];
        yak_array[j+1]=temp;
      }
    }
	}	
}

static void sortKarma(){     
	int n = sizeof(yak_array) / sizeof(YakInfo);
  YakInfo temp;
  for(int i=1;i<n;i++) {
    for(int j=0;j<n-i;j++) {
      if(yak_array[j].karma < yak_array[j+1].karma) {
        temp=yak_array[j];
        yak_array[j]=yak_array[j+1];
        yak_array[j+1]=temp;
      }
    }
	}	
}

static void sort_back_handler(ClickRecognizerRef recognizer, void *context) {
  window_stack_pop_all(true);
}

static void sort_select_callback(struct MenuLayer *s_menu_layer, MenuIndex *cell_index, void *callback_context) {
  sort_id = sort_array[cell_index->row];

	if(strcmp(sort_id.sType, "Hot") == 0) {
		sortKarma(yak_array);
		// yak_array = yak_array;
  } else {
  	sortAge(yak_array);
  	// yak_array = yak_array;
  }
  // Switch to yakview window
  window_stack_push(s_menu_window, true);
}

static uint16_t sort_get_sections_count_callback(struct MenuLayer *menulayer, uint16_t section_index, void *callback_context) {
  int count = sizeof(sort_array) / sizeof(SortType);
  return count;
}

static void sort_draw_row_handler(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
	sort_id = sort_array[cell_index->row];
	
  char* name = sort_id.sType;

  menu_cell_basic_draw(ctx, cell_layer, name, NULL, NULL);
}

static void sort_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
 
  s_sort_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_sort_layer, NULL, (MenuLayerCallbacks){
    .get_num_rows = sort_get_sections_count_callback,
    .draw_row = sort_draw_row_handler,
    .select_click = sort_select_callback
  });
  menu_layer_set_click_config_onto_window(s_sort_layer, s_sort_window);


  layer_add_child(window_layer, menu_layer_get_layer(s_sort_layer));
}

static void sort_window_unload(Window *window) {
  menu_layer_destroy(s_sort_layer);
}


// ___________________________________________________________________________________________________________
// MENU
// ___________________________________________________________________________________________________________

static void menu_back_handler(ClickRecognizerRef recognizer, void *context) {
  window_stack_pop_all(true);
}

static void  menu_click_config_onto_window(MenuLayer * menu_layer, struct Window * window) {
	window_stack_pop_all(true);
}

static void select_callback(struct MenuLayer *s_menu_layer, MenuIndex *cell_index, void *callback_context) {
  yak_id = yak_array[cell_index->row];
  arrayInd = cell_index->row;
  newPoints = yak_id.karma;
  // Switch to yakview window
  window_stack_push(s_yakview_window, true);
}

static uint16_t get_sections_count_callback(struct MenuLayer *menulayer, uint16_t section_index, void *callback_context) {
  int count = sizeof(yak_array) / sizeof(YakInfo);
  return count;
}

static void draw_row_handler(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
	yak_id = yak_array[cell_index->row];
	
  char* name = yak_id.yak_text;
  int points = yak_id.karma;

	strncpy(subyak, name, 15);

  // Using simple space padding between name and s_yak_text for appearance of edge-alignment
  snprintf(s_yak_text, sizeof(s_yak_text), "%s%s%s%d", subyak,"...", "", points);

  menu_cell_basic_draw(ctx, cell_layer, NULL, s_yak_text, NULL);
}

static void menu_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
 
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_rows = get_sections_count_callback,
    .draw_row = draw_row_handler,
    .select_click = select_callback
  });
  menu_layer_set_click_config_onto_window(s_menu_layer, s_menu_window);


  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void menu_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

// ___________________________________________________________________________________________________________
// YAK VIEW
// ___________________________________________________________________________________________________________

static void yak_back_handler(ClickRecognizerRef recognizer, void *context) {
  yak_array[arrayInd].karma = newPoints;
  window_stack_push(s_menu_window, true); 
}

static void yak_upvote_handler(ClickRecognizerRef recognizer, void *context) {
	newPoints = yak_id.karma+1;
	snprintf(strKarma,sizeof(strKarma), "%d", newPoints);
  text_layer_set_text(s_karma_text_layer, strKarma);
}

static void yak_downvote_handler(ClickRecognizerRef recognizer, void *context) {
	newPoints = yak_id.karma-1;
	snprintf(strKarma,sizeof(strKarma), "%d", newPoints);

  text_layer_set_text(s_karma_text_layer, strKarma);
}

static void yakview_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, yak_back_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, yak_downvote_handler);
  window_single_click_subscribe(BUTTON_ID_UP, yak_upvote_handler);
}

static void my_layer_draw(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);

	s_yakview_text_layer = text_layer_create(bounds);
  text_layer_set_text(s_yakview_text_layer, s_yakview_text);
  text_layer_set_text_alignment(s_yakview_text_layer, GTextAlignmentCenter);
  layer_add_child(layer, text_layer_get_layer(s_yakview_text_layer));
  
  s_yak_text_layer = text_layer_create(GRect(0, 12, bounds.size.w, 70));
  text_layer_set_text(s_yak_text_layer, yak_id.yak_text);
  text_layer_set_overflow_mode(s_yak_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(s_yak_text_layer, GTextAlignmentCenter);
  layer_add_child(layer, text_layer_get_layer(s_yak_text_layer));

	snprintf(strKarma,sizeof(strKarma), "%d", newPoints);

  s_karma_text_layer = text_layer_create(GRect(90, 110, 50, 40));
  text_layer_set_text(s_karma_text_layer, strKarma);
  text_layer_set_font(s_karma_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_karma_text_layer, GTextAlignmentLeft);
  layer_add_child(layer, text_layer_get_layer(s_karma_text_layer));

  snprintf(strAge,sizeof(strAge), "%d h", yak_id.age);

  s_age_text_layer = text_layer_create(GRect(20, 110, 50, 40));
  text_layer_set_text(s_age_text_layer, strAge);
  text_layer_set_font(s_age_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_age_text_layer, GTextAlignmentLeft);
  layer_add_child(layer, text_layer_get_layer(s_age_text_layer));

}

static void yakview_window_load(Window *window) {
  Layer * window_layer = window_get_root_layer(window);
  
  window_set_click_config_provider(window, yakview_click_config_provider);

  layer_set_update_proc(window_layer, my_layer_draw);
}

static void yakview_window_unload(Window *window) {
  text_layer_destroy(s_yakview_text_layer);
  text_layer_destroy(s_karma_text_layer);
  text_layer_destroy(s_yak_text_layer);
}

// ___________________________________________________________________________________________________________
// STARTUP
// ___________________________________________________________________________________________________________

static void startup_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Exit app after tea is done
  window_stack_push(s_sort_window, true);
}

static void startup_back_handler(ClickRecognizerRef recognizer, void *context) {
  // Exit app after tea is done
  window_stack_pop_all(true);
}

static void startup_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, startup_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, startup_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, startup_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, startup_back_handler);
}

static void startup_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	window_set_click_config_provider(window, startup_click_config_provider);

  // Bitmap layer for wakeup "tea is ready" image
  s_bitmap_layer = bitmap_layer_create(bounds);
  s_yak_bitmap = gbitmap_create_with_resource(RESOURCE_ID_YAK_LOGO);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_yak_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));

}

static void startup_window_unload(Window *window) {
  gbitmap_destroy(s_yak_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
}

static void init(void) {

	s_startup_window = window_create();
  window_set_window_handlers(s_startup_window, (WindowHandlers){
    .load = startup_window_load,
    .unload = startup_window_unload,
  });

  s_sort_window = window_create();
  window_set_window_handlers(s_sort_window, (WindowHandlers){
    .load = sort_window_load,
    .unload = sort_window_unload,
  });

  s_menu_window = window_create();
  window_set_window_handlers(s_menu_window, (WindowHandlers){
    .load = menu_window_load,
    .unload = menu_window_unload,
  });

  s_yakview_window = window_create();
  window_set_window_handlers(s_yakview_window, (WindowHandlers){
    .load = yakview_window_load,
    .unload = yakview_window_unload,
  });

  window_stack_push(s_startup_window, true);
}

static void deinit(void) {
  window_destroy(s_menu_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}