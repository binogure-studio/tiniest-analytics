#ifndef GOOGLEANALYTICS_H
#define GOOGLEANALYTICS_H

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>

#include "dictionary.h"
#include "object.h"
#include "reference.h"

class GoogleAnalytics : public Object
{
public:
  static GoogleAnalytics *get_singleton();
  static void reset_singleton();

  GoogleAnalytics();
  ~GoogleAnalytics();

  bool initialize(const String &trackingId, const String &uniqueClientId);
  void flush();
  void send_event_action(const String &category, const String &action);
  void send_event_action_label(const String &category, const String &action, const String &label);
  void send_event_action_label_value(const String &category, const String &action, const String &label, uint32_t value);

protected:
  static void _bind_methods();
  static GoogleAnalytics *singleton;

private:
  bool execute_curl_url(const String &url, ...);
  void replace_in_string(char *s, const int len, const char what, const char with);

  CURLM* curl_multi_handle;
  char base_url[2048] = {'\0'};


  OBJ_TYPE(GoogleAnalytics, Object);
  OBJ_CATEGORY("GoogleAnalytics");
};

#endif // GOOGLEANALYTICS_H
