/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "ani_timer.h"

void initialize_ani_timer_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
    ClassDB::register_class<AniTimer>();
}

void uninitialize_ani_timer_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}