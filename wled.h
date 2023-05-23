#ifndef WLED_H
#define WLED_H

struct wled {
  String wled_ip;
  String json_state;
  int click_type;
  unsigned long int controller_id;
};

#endif