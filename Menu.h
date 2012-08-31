// --------------------------------
// Condition menu
// --------------------------------

#ifdef CONDITION_MENU

	SEPARATOR
	DISABLED(0, "On Error")
	SEPARATOR
	ITEM(1, "File is Open")
	ITEM(2, "File is OK")
	SEPARATOR
	ITEM(3, "File Exists/has Read Access")

#endif

// --------------------------------
// Action menu
// --------------------------------

#ifdef ACTION_MENU

	SEPARATOR
	ITEM(0, "Open File Stream")
	ITEM(1, "Close File Stream")
	SEPARATOR
	ITEM(2, "Set Byte")
	ITEM(3, "Set Short")
	ITEM(4, "Set Int")
	ITEM(5, "Set Float")
	ITEM(6, "Set Double")
	ITEM(7, "Set String")
	SEPARATOR
	ITEM(8, "Clear Error State")
	SEPARATOR

#endif


// --------------------------------
// Expression menu
// --------------------------------

#ifdef EXPRESSION_MENU

	SEPARATOR
	ITEM(0, "Get Signed Byte")
	ITEM(9, "Get Unsigned Byte")
	ITEM(1, "Get Signed Short")
	ITEM(10,"Get Unsigned Short")
	ITEM(2, "Get Int")
	ITEM(3, "Get Float")
	ITEM(4, "Get Double")
	ITEM(5, "Get String")
	SEPARATOR
	ITEM(6, "Get Read Cursor Position")
	ITEM(7, "Get Write Cursor Position")
	ITEM(8, "Get File Size")
	SEPARATOR

#endif