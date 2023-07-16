/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "animation_iso.h"

void initialize_animation_iso_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
    ClassDB::register_class<AnimationIso>();
}

void uninitialize_animation_iso_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}