#ifndef GOOGLEANALYTICS_H
#define GOOGLEANALYTICS_H

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "curl/curl.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

class GoogleAnalytics : public Object
{
public:
  static GoogleAnalytics *get_singleton();
  static void reset_singleton();

  GoogleAnalytics();
  ~GoogleAnalytics();

  bool initialize(const char* trackingId, const char* uniqueClientId);
  void flush();
  void send_event_action_label_value(const char* category, const char* action);
  void send_event_action_label(const char* category, const char* action, const char* label);
  void send_event_action(const char* category, const char* action, const char* label, unsigned int value);

protected:
  static void _bind_methods();
  static GoogleAnalytics *singleton;

  CURLM* curl_multi_handle;
  char base_url[2048] = {'\0'};


  OBJ_TYPE(GoogleAnalytics, Object);
  OBJ_CATEGORY("GoogleAnalytics");
};

#endif // GOOGLEANALYTICS_H
