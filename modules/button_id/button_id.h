#ifndef BUTTON_ID_CLASS_H
#define BUTTON_ID_CLASS_H

#include "scene/gui/button.h"

class ButtonID : public Button
{
	GDCLASS(ButtonID, Button);
	int button_id;
	
	protected:
		static void _bind_methods();
		void _notification(int p_what);
		
	public:
		ButtonID();

		//getters and setters
		void setButtonID(int buttonId) { this->button_id = buttonId; }
		int getButtonID() { return button_id; }
};

#endif //BUTTON_ID_CLASS_H
