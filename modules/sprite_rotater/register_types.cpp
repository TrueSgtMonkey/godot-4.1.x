/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "sprite_rotater.h"

void initialize_sprite_rotater_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
    ClassDB::register_class<SpriteRotater>();
}

void uninitialize_sprite_rotater_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}