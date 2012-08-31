// --------------------
// Object information
// --------------------

// Basic object details
#define	ObjectName			"File Stream Object"
#define	ObjectAuthor		"LB"
#define	ObjectCopyright		"Copyright © 2011 LB-Stuff"
#define	ObjectComment		"This object allows you to open and manage file streams without loading the files into memory.\nMade by request for Alonso.\nIcon thanks to Alonso."
#define	ObjectURL			"http://www.LB-Stuff.com/"
#define	ObjectHelp			""

// If you register your object with Clickteam, change this to the ID you were given
#define ObjectRegID			REGID_PRIVATE

// Change N,O,N,E to 4 unique characters (MMF currently still uses this to keep track)
#define	IDENTIFIER			MAKEID(F,S,T,R) //File STReam

// --------------------
// Version information
// --------------------

// PRODUCT_VERSION_TGF or PRODUCT_VERSION_DEVELOPER
#define ForVersion			PRODUCT_VERSION_STANDARD

// Set this to the latest MMF build out when you build the object
#define	MinimumBuild		251

// --------------------
// Beta information
// --------------------

// #define BETA
// #define POPUP_ON_DROP
// #define POPUP_ON_EXE
// #define POPUP_ON_BUILD
// #define POPUP_MESSAGE	"This is a beta extension; use with caution!\nIf you find a crash, bug, or any other error, please report it."

// --------------------
// Handling priority
// --------------------
// If this extension will handle windows messages, specify the priority
// of the handling procedure, 0 = low and 255 = very high

#define	WINDOWPROC_PRIORITY 100
