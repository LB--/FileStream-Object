// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================

#include "Common.h"
#include "ParamHelp.h"

// ============================================================================
//
// CONDITIONS
// 
// ============================================================================

CONDITION(
	/* ID */			0,
	/* Name */			"%o: On Error",
	/* Flags */			0,
	/* Params */		(0)
) {
	return(true);
}

CONDITION(
	/* ID */			1,
	/* Name */			"%o: File in Slot %0 is Open",
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(1,PARAM_NUMBER,"Slot to Check")
) {
	int Slot = lParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		return(rdPtr->Files[Slot]->is_open());
	}
	return(false);
}

CONDITION(
	/* ID */			2,
	/* Name */			"%o: File in Slot %0 is OK",
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(1,PARAM_NUMBER,"Slot to Check")
) {
	int Slot = lParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		return(rdPtr->Files[Slot]->good());
	}
	return(false);
}

CONDITION(
	/* ID */			3,
	/* Name */			"%o: File path %0 exists/is readable",
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(1,PARAM_STRING,"File to check for Existence/Read Access")
) {
	return(ifstream(sParam()).is_open());
}

// ============================================================================
//
// ACTIONS
// 
// ============================================================================

ACTION(
	/* ID */			0,
	/* Name */			"Open File Stream on %0 in Slot %1 with flags %2",
	/* Flags */			0,
	/* Params */		(3,PARAM_STRING,"File to Open",PARAM_NUMBER,"Slot to open File in (will close old stream in slot)",PARAM_NUMBER,"Flags (add together): 1 = read, 2 = write, 4 = clear file contents, 8 = append to file")
) {
	string File = StrParam();
	int Slot = lParam();
	int Flags = lParam();
	ios::openmode flags = ios::binary;
	if(Flags & 1){ flags |= ios::in; }
	if(Flags & 2){ flags |= ios::out; }
	if(Flags & 4){ flags |= ios::trunc; }
	if(Flags & 8){ flags |= ios::app; }
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end()){ delete rdPtr->Files[Slot]; }
	rdPtr->Files[Slot] = new fstream(File.c_str(), flags);
}

ACTION(
	/* ID */			1,
	/* Name */			"Close File Stream on %0",
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,"Slot to Close")
) {
	int Slot = lParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		delete rdPtr->Files[Slot];
		rdPtr->Files.erase(Slot);
	}
}

ACTION(
	/* ID */			2,
	/* Name */			"Set Byte to %2 at %1 in Slot %0",
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,"Slot",PARAM_NUMBER,"Position in File (be careful not to exceed the file size)",PARAM_NUMBER,"New Value of Byte")
) {
	int Slot = lParam();
	unsigned long Pos = unsigned long(lParam());
	char Val = char(lParam());
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekp(Pos);
		rdPtr->Files[Slot]->write(&Val, sizeof(Val));
	}
}

ACTION(
	/* ID */			3,
	/* Name */			"Set Short to %2 at %1 in Slot %0",
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,"Slot",PARAM_NUMBER,"Position in File (be careful not to exceed the file size)",PARAM_NUMBER,"New Value of Short")
) {
	int Slot = lParam();
	unsigned long Pos = unsigned long(lParam());
	short Val = short(lParam());
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekp(Pos);
		rdPtr->Files[Slot]->write(reinterpret_cast<char*>(&Val), sizeof(Val));
	}
}

ACTION(
	/* ID */			4,
	/* Name */			"Set Int to %2 at %1 in Slot %0",
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,"Slot",PARAM_NUMBER,"Position in File (be careful not to exceed the file size)",PARAM_NUMBER,"New Value of Int")
) {
	int Slot = lParam();
	unsigned long Pos = unsigned long(lParam());
	long Val = lParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekp(Pos);
		rdPtr->Files[Slot]->write(reinterpret_cast<char*>(&Val), sizeof(Val));
	}
}

ACTION(
	/* ID */			5,
	/* Name */			"Set Float to %2 at %1 in Slot %0",
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,"Slot",PARAM_NUMBER,"Position in File (be careful not to exceed the file size)",PARAM_NUMBER,"New Value of Float")
) {
	int Slot = lParam();
	unsigned long Pos = unsigned long(lParam());
	float Val = fParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekp(Pos);
		rdPtr->Files[Slot]->write(reinterpret_cast<char*>(&Val), sizeof(Val));
	}
}

ACTION(
	/* ID */			6,
	/* Name */			"Set Double to %2 at %1 in Slot %0",
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,"Slot",PARAM_NUMBER,"Position in File (be careful not to exceed the file size)",PARAM_NUMBER,"New Value of Double")
) {
	int Slot = lParam();
	unsigned long Pos = unsigned long(lParam());
	double Val = fParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekp(Pos);
		rdPtr->Files[Slot]->write(reinterpret_cast<char*>(&Val), sizeof(Val));
	}
}

ACTION(
	/* ID */			7,
	/* Name */			"Set String to %2 (Null Terminate = %3) at %1 in Slot %0",
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,"Slot",PARAM_NUMBER,"Position in File (be careful not to exceed the file size)",PARAM_STRING,"New Value of String",PARAM_NUMBER,"Null Terminate? 0 = No, anything else = Yes")
) {
	int Slot = lParam();
	unsigned long Pos = unsigned long(lParam());
	string Val = StrParam();
	long NullT = ((lParam()) ? 1 : 0);
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekp(Pos);
		rdPtr->Files[Slot]->write(Val.c_str(), Val.length()+NullT);
	}
}

ACTION(
	/* ID */			8,
	/* Name */			"Clear Error State in Slot %0",
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,"Slot")
) {
	int Slot = lParam();
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->clear();
	}
}

// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

EXPRESSION(
	/* ID */			0,
	/* Name */			"ByteAt(",
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	char Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	return(Val);
}

EXPRESSION(
	/* ID */			1,
	/* Name */			"ShortAt(",
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	short Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	return(Val);
}

EXPRESSION(
	/* ID */			2,
	/* Name */			"IntAt(",
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	long Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	return(Val);
}

EXPRESSION(
	/* ID */			3,
	/* Name */			"FloatAt(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	float Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	ReturnFloat(Val);
}

EXPRESSION(
	/* ID */			4,
	/* Name */			"DoubleAt(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	double Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	ReturnFloat(float(Val));
}

EXPRESSION(
	/* ID */			5,
	/* Name */			"StringAt$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)",EXPPARAM_NUMBER,"Length (-1 for Null Terminated)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	unsigned long NullT = unsigned long(xlParam());
	string Val;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		char c;
		if(NullT == unsigned long(-1))
		{
			while(rdPtr->Files[Slot]->get(c), c)
			{
				Val += c;
			}
		}
		else
		{
			while(rdPtr->Files[Slot]->get(c), Val.length() < NullT)
			{
				Val += c;
			}
		}
	}

	ReturnStringSafe(Val.c_str());
}

EXPRESSION(
	/* ID */			6,
	/* Name */			"ReadCursorPos(",
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,"Slot")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(-1);
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		Pos = rdPtr->Files[Slot]->tellg();
	}

	return(Pos);
}

EXPRESSION(
	/* ID */			7,
	/* Name */			"WriteCursorPos(",
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,"Slot")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(-1);
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		Pos = rdPtr->Files[Slot]->tellp();
	}

	return(Pos);
}

EXPRESSION(
	/* ID */			8,
	/* Name */			"FileSize(",
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,"Slot")
) {
	int Slot = xlParam();
	unsigned long Size = unsigned long(-1);
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		unsigned long g = rdPtr->Files[Slot]->tellg();
		rdPtr->Files[Slot]->seekg(0, ios::end);
		Size = rdPtr->Files[Slot]->tellg();
		rdPtr->Files[Slot]->seekg(g);
	}

	return(Size);
}

EXPRESSION(
	/* ID */			9,
	/* Name */			"UnsignedByteAt(",
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	unsigned char Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	return(Val);
}

EXPRESSION(
	/* ID */			10,
	/* Name */			"UnsignedShortAt(",
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,"Slot",EXPPARAM_NUMBER,"Position in File (be careful not to exceed the file size)")
) {
	int Slot = xlParam();
	unsigned long Pos = unsigned long(xlParam());
	unsigned short Val = 0;
	if(rdPtr->Files.find(Slot) != rdPtr->Files.end())
	{
		rdPtr->Files[Slot]->seekg(Pos);
		rdPtr->Files[Slot]->read(reinterpret_cast<char*>(&Val), sizeof(Val));
	}

	return(Val);
}