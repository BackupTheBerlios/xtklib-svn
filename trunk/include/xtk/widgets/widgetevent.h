/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2005 Mario Casciaro (xshadow@email.it)
*
* Licensed under: 
*   - Apache License, Version 2.0 or
*   - GNU General Public License (GPL)
* You should have received at least one copy of them along with this program.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
* PURPOSE ARE DISCLAIMED.SEE YOUR CHOOSEN LICENSE FOR MORE DETAILS.
*/

/**
* @file widgetevent.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGETEVENT_H
#define XTK_WIDGETEVENT_H

#include "../globals.h"
#include "../base/event.h"
#include "../base/string.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{


//forward
class xWidget;
class xWindow;


/**
 * 
 */
enum xWidgetEventID
{
	//xComponentEvent
	XWE_COMPONENT_SHOWN,
	XWE_COMPONENT_HIDDEN,
	XWE_COMPONENT_RESIZED,
	XWE_COMPONENT_MOVED,
	
	//xFocusEvent
	XWE_FOCUS_GAINED,
	XWE_FOCUS_LOST,
	
	//xWindowEvent
	XWE_WINDOW_ACTIVATED,
	XWE_WINDOW_CLOSED,
	XWE_WINDOW_CLOSING,
	XWE_WINDOW_DEACTIVATED,
	XWE_WINDOW_DEICONIFIED,
	XWE_WINDOW_FIRST,
	XWE_WINDOW_GAINED_FOCUS,
	XWE_WINDOW_ICONIFIED,
	XWE_WINDOW_LAST,
	XWE_WINDOW_LOST_FOCUS,
	XWE_WINDOW_OPENED,
	XWE_WINDOW_STATE_CHANGED,
	
	//xContainerEvent
	XWE_CONTAINER_ADDED,
	XWE_CONTAINER_REMOVED,
	
	//xPaintEvent
	XWE_PAINT_PAINT,
	XWE_PAINT_REPAINT,
	
	//xKeyEvent
	XWE_KEY_PRESSED,
	XWE_KEY_RELEASED,
	XWE_KEY_TYPED,
	
	//xMouseEvent
	XWE_MOUSE_CLICKED,
	XWE_MOUSE_DOUBLECLICKED,
	XWE_MOUSE_DRAGGED,
	XWE_MOUSE_MOVED,
	XWE_MOUSE_PRESSED,
	XWE_MOUSE_RELEASED,
	XWE_MOUSE_WHEEL,
	
	//xActionEvent
	XWE_ACTION_PERFORMED,
	
	//xActionEvent
};



/**
 * The root event class for all xWidget events
 */
class XTKAPI xWidgetEvent : public xEvent
{
protected:
	xWidgetEventID		m_id;
	
public:
	virtual ~xWidgetEvent(){}
	
	enum EventMask
	{
		//!The event mask for selecting action events.
		ACTION_EVENT_MASK = 1,
	
		//!The event mask for selecting widget events.
		WIDGET_EVENT_MASK = 2,
		
		//!The event mask for selecting focus events.
		FOCUS_EVENT_MASK = 4,
		
		//!The event mask for selecting input method events.
		INPUT_METHOD_EVENT_MASK = 8,
		
		//!The event mask for selecting item events.
		ITEM_EVENT_MASK = 16,
		
		//!The event mask for selecting key events.
		KEY_EVENT_MASK = 32,
		
		//!The event mask for selecting mouse events.
		MOUSE_EVENT_MASK = 64,
		
		//!The event mask for selecting mouse motion events.
		MOUSE_MOTION_EVENT_MASK = 128,
		
		//!The event mask for selecting mouse wheel events.
		MOUSE_WHEEL_EVENT_MASK = 256,
		
		//!The event mask for selecting paint events.
		PAINT_EVENT_MASK = 512,
		
		//!The event mask for selecting text events.
		TEXT_EVENT_MASK = 1024,
		
		//!The event mask for selecting window events.
		WINDOW_EVENT_MASK = 2048
	};
	
	
	/**
	 * Ctor
	 */
	xWidgetEvent(xWidget* source,xWidgetEventID id) : xEvent((xObject*)source){m_id = id;}
	
	/**
	 * Returns the event type id.
	 */
	xWidgetEventID getID(){return m_id;}
	
	/**
	* Returns the originator of the event.
	*/
	xWidget& getWidget();
};


/**
 * A low-level event which indicates that a widget moved, changed size, or changed visibility.
 */
class XTKAPI xComponentEvent : public xWidgetEvent
{
public:
	xComponentEvent(xWidget* widget,xWidgetEventID id) : xWidgetEvent(widget,id){}
	virtual ~xComponentEvent(){}
}; 
 
 
/**
 * A low-level event which indicates that a Component has gained or lost the input focus.
 */
class XTKAPI xFocusEvent : public xComponentEvent
{
public:
	xFocusEvent(xWidget* widget,xWidgetEventID id) : xComponentEvent(widget,id){}
	virtual ~xFocusEvent(){}
};


/**
 * A low-level event that indicates that a window has changed its status.
 */
class XTKAPI xWindowEvent : public xComponentEvent
{
public:
	xWindowEvent(xWidget* widget,xWidgetEventID id) : xComponentEvent(widget,id){}
	virtual ~xWindowEvent(){}
	
	xWindow& getWindow();
};
 
 
/**
 * A low-level event which indicates that a container's contents changed because a 
 * component was added or removed.
 */
class XTKAPI xContainerEvent : public xComponentEvent
{
protected:
	xWidget& m_child;
	
public:
	xContainerEvent(xWidget* widget,xWidgetEventID id,xWidget* child) 
	: xComponentEvent(widget,id),m_child(*child){}
	virtual ~xContainerEvent(){}
	
	/**
	 * Returns the widget that was affected by the event.
	 */
	xWidget& getChild(){return m_child;}
}; 


/**
 * The component-level paint event.
 */
class XTKAPI xPaintEvent : public xComponentEvent
{
public:
	xPaintEvent(xWidget* widget,xWidgetEventID id) : xComponentEvent(widget,id){}
	virtual ~xPaintEvent(){}
}; 


/**
 * The root event class for all widget-level input events.
 */
class XTKAPI xInputEvent : public xComponentEvent
{
protected:
	int m_modifiers;
public:
	enum ModifierMask
	{
		MODIFIER_MASK_ALT_DOWN = 1,
		MODIFIER_MASK_ALT_GRAPH_DOWN = 2,
		MODIFIER_MASK_BUTTON1_DOWN = 4,
		MODIFIER_MASK_BUTTON2_DOWN = 8,
		MODIFIER_MASK_BUTTON3_DOWN = 16,
		MODIFIER_MASK_CTRL_DOWN = 32,
		MODIFIER_MASK_META_DOWN = 64,
		MODIFIER_MASK_SHIFT_DOWN = 128,
	};

	xInputEvent(xWidget* widget,xWidgetEventID id,int modifiers) : xComponentEvent(widget,id)
	{m_modifiers = modifiers;}
	virtual ~xInputEvent(){}
	
	int getModifiers(){return m_modifiers;}
}; 


/**
 * An event which indicates that a keystroke occurred in a component.
 */
class XTKAPI xKeyEvent : public xInputEvent
{
public:

	enum KeyCode
	{
		KC_0,
		KC_1,
		KC_2,
		KC_3,
		KC_4,
		KC_5,
		KC_6,
		KC_7,
		KC_8,
		KC_9,
		KC_A,
		KC_ALT,
		KC_ALT_GRAPH,
		KC_AMPERSAND,
		KC_ASTERISK,
		KC_AT,
		KC_B,
		KC_BACK_QUOTE,
		KC_BACK_SLASH,
		KC_BACK_SPACE,
		KC_BRACELEFT,
		KC_BRACERIGHT,
		KC_C,
		KC_CANCEL,
		KC_CAPS_LOCK,
		KC_CIRCUMFLEX,
		KC_CLEAR,
		KC_CLOSE_BRACKET,
		KC_COLON,
		KC_COMMA,
		KC_CONTROL,
		KC_COPY,
		KC_CUT,
		KC_D,
		KC_DECIMAL,
		KC_DELETE,
		KC_DIVIDE,
		KC_DOLLAR,
		KC_DOWN,
		KC_E,
		KC_END,
		KC_ENTER,
		KC_EQUALS,
		KC_ESCAPE,
		KC_EURO_SIGN,
		KC_EXCLAMATION_MARK,
		KC_F,
		KC_F1,
		KC_F10,
		KC_F11,
		KC_F12,
		KC_F2,
		KC_F3,
		KC_F4,
		KC_F5,
		KC_F6,
		KC_F7,
		KC_F8,
		KC_F9,
		KC_FINAL,
		KC_FIND,
		KC_G,
		KC_GREATER,
		KC_H,
		KC_HELP,
		KC_HOME,
		KC_I,
		KC_INSERT,
		KC_J,
		KC_K,
		KC_KP_DOWN,
		//!Constant for the numeric keypad left arrow key.
		KC_KP_LEFT,
		//!Constant for the numeric keypad right arrow key.
		KC_KP_RIGHT,
		//!Constant for the numeric keypad up arrow key.
		KC_KP_UP,
		KC_L,
		//!Constant for the non-numpad left arrow key.
		KC_LEFT,
		//!Constant for the "(" key.
		KC_LEFT_PARENTHESIS,
		KC_LESS,
		KC_M,
		KC_META,
		//!Constant for the "-" key.
		KC_MINUS,
		KC_MULTIPLY,
		KC_N,
		KC_NUM_LOCK,
		//!Constant for the "#" key.
		KC_NUMBER_SIGN,
		KC_NUMPAD0,
		KC_NUMPAD1,
		KC_NUMPAD2,
		KC_NUMPAD3,
		KC_NUMPAD4,
		KC_NUMPAD5,
		KC_NUMPAD6,
		KC_NUMPAD7,
		KC_NUMPAD8,
		KC_NUMPAD9,
		KC_O,
		KC_OPEN_BRACKET,
		KC_P,
		KC_PAGE_DOWN,
		KC_PAGE_UP,
		KC_PASTE,
		KC_PAUSE,
		KC_PERIOD,
		KC_PLUS,
		KC_PRINTSCREEN,
		KC_Q,
		KC_QUOTE,
		KC_QUOTEDBL,
		KC_R,
		//!Constant for the non-numpad right arrow key.
		KC_RIGHT,
		//!Constant for the ")" key.
		KC_RIGHT_PARENTHESIS,
		KC_S,
		KC_SCROLL_LOCK,
		KC_SEMICOLON,
		//!Constant for the Numpad Separator key.
		KC_SEPARATOR,
		KC_SHIFT,
		KC_SLASH,
		KC_SPACE,
		KC_STOP,
		KC_T,
		KC_TAB,
		KC_U,
		//!This value is used to indicate that the keyCode is unknown.
		KC_UNDEFINED,
		//!Constant for the "_" key.
		KC_UNDERSCORE,
		KC_UNDO,
		//!Constant for the non-numpad up arrow key.
		KC_UP,
		KC_V,
		KC_W,
		KC_X,
		KC_Y,
		KC_Z
	};
	
	
	
protected:
	KeyCode		m_keycode;

public:
	xKeyEvent(xWidget* widget,xWidgetEventID id,int modifiers,KeyCode keycode) 
	: xInputEvent(widget,id,modifiers)
	{m_keycode = keycode;}
	
	virtual ~xKeyEvent(){}
}; 

 
 
/**
 * An event which indicates that a mouse action occurred in a widget. 
 */
class XTKAPI xMouseEvent : public xInputEvent
{
public:
	enum MouseButton
	{
		NOBUTTON,
		BUTTON1,
		BUTTON2,
		BUTTON3,
	};
	
protected:
	MouseButton	m_button;
	int m_x;
	int m_y;
	
public:
	xMouseEvent(xWidget* widget,xWidgetEventID id,int modifiers,int x,int y,MouseButton button) 
	: xInputEvent(widget,id,modifiers)
	{
		m_button = button;
		m_x = x;
		m_y = y;
	}
	virtual ~xMouseEvent(){}
	
	MouseButton getButton(){return m_button;}
	int getX(){return m_x;}
	int getY(){return m_y;}
};


/**
 * A semantic event which indicates that a component-defined action occured.
 */
class XTKAPI xActionEvent : public xComponentEvent
{
public:
	xActionEvent(xWidget* widget,xWidgetEventID id) : xComponentEvent(widget,id)
	{}
	
	virtual ~xActionEvent(){}
};


}//namespace
#endif //XTK_USE_WIDGETS


#endif //XTK_WIDGETEVENT_H
