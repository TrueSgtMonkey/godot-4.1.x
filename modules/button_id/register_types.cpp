/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "button_id.h"

void initialize_button_id_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
    ClassDB::register_class<ButtonID>();
}

void uninitialize_button_id_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}