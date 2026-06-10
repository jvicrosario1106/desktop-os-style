#include "imguielement.h"

void IMGUIElement::ToggleShow() {
	this->show_element = !this->show_element;
}

bool IMGUIElement::GetShowElement() const {
	return this->show_element;
}