#include "googleanalytics.h"

GoogleAnalytics *GoogleAnalytics::singleton = NULL;

GoogleAnalytics::GoogleAnalytics() {
  curl_multi_handle = NULL;
  singleton = this;
}

GoogleAnalytics::~GoogleAnalytics() {
	if (curl_multi_handle) {
		// one last update to remove handles from stack if they're ready
		flush();
		curl_multi_cleanup(curl_multi_handle);
		curl_multi_handle = NULL;
	}

	singleton = NULL;
}

GoogleAnalytics *GoogleAnalytics::get_singleton() {
  if (GoogleAnalytics::singleton == NULL) {
    GoogleAnalytics::singleton = new GoogleAnalytics();
  }

  return GoogleAnalytics::singleton;
}

void GoogleAnalytics::reset_singleton() {
  delete GoogleAnalytics::singleton;

  GoogleAnalytics::singleton = NULL;
}

// utility function, used to replace spaces with pluses for URLs
void replace_in_string(char *s, const int len, const char what, const char with) {
	for (int i = 0; i < len; ++i) {
		if (s[i] == what) {
			s[i] = with;
		}
	}
}

// utility function, used to send the HTTP get
bool execute_curl_url(const char* url, ...) {
	if (!curl_multi_handle) {
		return false;
	}

	va_list argptr;
	va_start(argptr, url);
	char strAddr[2048] = {'\0'};
	int slen = vsprintf(strAddr, url, argptr);
	va_end(argptr);

	replace_in_string(strAddr, slen, ' ', '+');

	CURL* pCurlHandle = curl_easy_init();
	curl_easy_setopt(pCurlHandle, CURLOPT_URL, strAddr);
	curl_easy_setopt(pCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(pCurlHandle, CURLOPT_TIMEOUT, 20);
	CURLMcode result = curl_multi_add_handle(curl_multi_handle, pCurlHandle);

	return (result == CURLM_OK);
}

bool initialize(const char* trackingId, const char* uniqueClientId) {
	bool initialized = false;

	curl_global_init(CURL_GLOBAL_ALL);
	curl_multi_handle = curl_multi_init();

	if (curl_multi_handle) {
		initialized = true;

		sprintf(base_url, "http://www.google-analytics.com/collect?v=1&tid=%s&cid=%s", trackingId, uniqueClientId);
	}

	return initialized;
}

void send_event_action_label_value(const char* category, const char* action, const char* label, unsigned int value) {
	execute_curl_url("%s&t=event&ec=%s&ea=%s&el=%s&ev=%u&z=%d", base_url, category, action, label, value, rand());
}

void send_event_action_label(const char* category, const char* action, const char* label) {
	execute_curl_url("%s&t=event&ec=%s&ea=%s&el=%s&z=%d", base_url, category, action, label, rand());
}

void send_event_action(const char* category, const char* action) {
	execute_curl_url("%s&t=event&ec=%s&ea=%s&z=%d", base_url, category, action, rand());
}

void flush() {
	if (!curl_multi_handle) {
		return;
	}

	int stillRunning = 0;

	curl_multi_perform(curl_multi_handle, &stillRunning);

	CURLMsg* pMsg = NULL;

	do {
		int msgsInQueue = 0;

		pMsg = curl_multi_info_read(curl_multi_handle, &msgsInQueue);

		if (pMsg && (pMsg->msg == CURLMSG_DONE)) {
			long response_code;
			curl_easy_getinfo(pMsg->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
			if (response_code != 200) {
				const char *urlp;
				curl_easy_getinfo(pMsg->easy_handle, CURLINFO_EFFECTIVE_URL, &urlp);

				if (response_code == 200) {
					emit_signal("event_sent");
				} else {
					char strerr[2048];
					
					sprintf(strerr, "[Error] flush() failed for URL '%s' with error %ld\n", urlp ? urlp : "?", response_code);					
					emit_signal("event_failed", strerr);
				}
			}
			curl_multi_remove_handle(curl_multi_handle, pMsg->easy_handle);
			curl_easy_cleanup(pMsg->easy_handle);
		}
	} while (pMsg);
}


void GoogleAnalytics::_bind_methods() {
	ObjectTypeDB::bind_method("initialize", &GoogleAnalytics::initialize);
	ObjectTypeDB::bind_method("send_event_action", &GoogleAnalytics::send_event_action);
	ObjectTypeDB::bind_method("send_event_action_label", &GoogleAnalytics::send_event_action_label);
	ObjectTypeDB::bind_method("send_event_action_label_value", &GoogleAnalytics::send_event_action_label_value);
	ObjectTypeDB::bind_method("flush", &GoogleAnalytics::flush);

  // Signals
  ADD_SIGNAL(MethodInfo("event_sent"));
  ADD_SIGNAL(MethodInfo("event_failed", PropertyInfo(Variant::STRING, "message")));
}
