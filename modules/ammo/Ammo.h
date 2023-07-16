#ifndef AMMO_CLASS_H
#define AMMO_CLASS_H

#include "core/object/ref_counted.h"

class Ammo : public RefCounted
{
	GDCLASS(Ammo, RefCounted);

	int ammo;
	int max_ammo;
	
	protected:
		static void _bind_methods();
		
	public:
		Ammo();
		bool useAmmo(int ammoAmount);
		bool addAmmo(int ammoAmount);
		void setMaxAmmo(int ammoAmount);
		int getMaxAmmo();
		int getAmmo();
		void setAmmo(int ammoAmount);
};

#endif //AMMO_CLASS_H