#include "globals.h"
#include "object_type_db.h"
#include "register_types.h"

#include "googleanalytics.h"

void register_googleanalytics_types() {
  Globals::get_singleton()->add_singleton(Globals::Singleton("GoogleAnalytics", GoogleAnalytics::get_singleton()));

  ObjectTypeDB::register_virtual_type<GoogleAnalytics>();
}

void unregister_googleanalytics_types() {
  GoogleAnalytics::reset_singleton();
}
