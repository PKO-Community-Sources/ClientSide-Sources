/**********************************************************************
 *<
	FILE: notify.h

	DESCRIPTION: Include file for event notification support

	CREATED BY: Tom Hudson

	HISTORY: Created 8 April 1995

 *>	Copyright (c) 1995, All Rights Reserved.
 **********************************************************************/

#ifndef _NOTIFY_H_

#define _NOTIFY_H_

// Pre-defined Jaguar system notification codes

#define NOTIFY_UNITS_CHANGE				0x00000001
#define NOTIFY_TIMEUNITS_CHANGE	 		0x00000002
#define NOTIFY_VIEWPORT_CHANGE			0x00000003
#define NOTIFY_SPACEMODE_CHANGE	 		0x00000004
#define NOTIFY_SYSTEM_PRE_RESET	 		0x00000005	// Sent before system is reset
#define NOTIFY_SYSTEM_POST_RESET 		0x00000006	// Sent after system is reset
#define NOTIFY_SYSTEM_PRE_NEW			0x00000007	// Sent before system is NEW'd-out
#define NOTIFY_SYSTEM_POST_NEW			0x00000008	// Sent after system is NEW'd-out
#define NOTIFY_FILE_PRE_OPEN			0x00000009	// Sent before a new file is opened
#define NOTIFY_FILE_POST_OPEN			0x0000000A	// Sent after a new file is opened successfully
#define NOTIFY_FILE_PRE_MERGE			0x0000000B	// Sent before a file is merged
#define NOTIFY_FILE_POST_MERGE			0x0000000C	// Sent after a file is merged successfully
#define NOTIFY_FILE_PRE_SAVE			0x0000000D	// Sent before a file is saved ( callParam is TCHAR * to file name )
#define NOTIFY_FILE_POST_SAVE			0x0000000E	// Sent after a file is saved ( callParam is TCHAR * to file name )
#define NOTIFY_FILE_PRE_SAVE_OLD 		0x00000010	// Sent before an old version file is saved
#define NOTIFY_FILE_POST_SAVE_OLD		0x00000011	// Sent after an old version file is saved
#define NOTIFY_SELECTIONSET_CHANGED		0x00000012	// Sent after the selection set has changed
#define NOTIFY_BITMAP_CHANGED			0x00000013	// Sent after the bitmap is reloaded ( callParam is TCHAR * to bitmap file name )
#define NOTIFY_PRE_RENDER				0x00000014	// Sent before rendering is started
#define NOTIFY_POST_RENDER				0x00000015	// Sent after rendering has finished
// Note: The NOTIFY_PRE_RENDERFRAME and NOTIFY_POST_RENDERFRAME callbacks are for internal use.
// The scene is to be considered read-only at those callbacks.
#define NOTIFY_PRE_RENDERFRAME			0x00000016	// Sent before rendering each frame ( callParam is RenderGlobalContext* )
#define NOTIFY_POST_RENDERFRAME			0x00000017	// Sent after rendering each frame  ( callParam is RenderGlobalContext* )
#define NOTIFY_PRE_IMPORT				0x00000018	// Sent before a file is imported
#define NOTIFY_POST_IMPORT				0x00000019	// Sent after a file is imported
#define NOTIFY_IMPORT_FAILED			0x0000001A	// Sent if import fails
#define NOTIFY_PRE_EXPORT				0x0000001B	// Sent before a file is exported
#define NOTIFY_POST_EXPORT				0x0000001C	// Sent after a file is exported
#define NOTIFY_EXPORT_FAILED			0x0000001D	// Sent if export fails
#define NOTIFY_NODE_RENAMED				0x0000001E	// Sent if node renamed. (call Param is pointer to struct{ TCHAR* oldname; TCHAR* newname; } }
#define NOTIFY_PRE_PROGRESS				0x0000001F	// Sent before the progress bar is displayed (so you can hide your window)
#define NOTIFY_POST_PROGRESS			0x00000020	// Sent after the progress bar is finished (so you can show your window again)
#define NOTIFY_MODPANEL_SEL_CHANGED		0x00000021	// Sent when the modify panel focuses on a new object (via open Mod panel or changing selection)
#define NOTIFY_HEIGHT_CHANGED			0x00000022	// VIZ Sent when the user operates the height menu
#define NOTIFY_RENDPARAM_CHANGED		0x00000023	// VIZ Sent when the common renderer parameters have changed
#define NOTIFY_MATLIB_PRE_OPEN			0x00000024	// Sent before loading a material library
#define NOTIFY_MATLIB_POST_OPEN			0x00000025	// Sent after loading a material library (call Param is ptr to MtlBaseLib if success, else NULL)
#define NOTIFY_MATLIB_PRE_SAVE			0x00000026	// Sent before saving a material library
#define NOTIFY_MATLIB_POST_SAVE			0x00000027	// Sent after saving a material library
#define NOTIFY_MATLIB_PRE_MERGE			0x00000028	// Sent before merging a material library
#define NOTIFY_MATLIB_POST_MERGE		0x00000029	// Sent after merging a material library
#define NOTIFY_FILELINK_PRE_BIND		0x00000030	// VIZ Sent before a file link bind
#define NOTIFY_FILELINK_POST_BIND		0x00000031	// VIZ Sent after a file link bind
#define NOTIFY_FILELINK_PRE_DETACH		0x00000032	// VIZ Sent before a file link detach
#define NOTIFY_FILELINK_POST_DETACH		0x00000033	// VIZ Sent after a file link detach
#define NOTIFY_FILELINK_PRE_RELOAD		0x00000034	// VIZ Sent before a file link reload (partial, full, or dynamic)
#define NOTIFY_FILELINK_POST_RELOAD		0x00000035	// VIZ Sent after a file link reload (partial, full, or dynamic)
#define NOTIFY_FILELINK_PRE_ATTACH		0x00000036	// VIZ Sent before a file link attach
#define NOTIFY_FILELINK_POST_ATTACH		0x00000037	// VIZ Sent after a file link attach
#define NOTIFY_AB_NAVIGATE_URL			0x00000038  // VIZ request to load a URL into the Asset Browser
#define NOTIFY_RENDER_PREEVAL			0x00000039  // Sent before the render start evaluating objects

#define NOTIFY_NODE_CREATED				0x0000003A  // Sent when a node is created (callParam is pointer to node)
#define NOTIFY_NODE_LINKED				0x0000003B  // Sent when a node is linked (callParam is pointer to node)
#define NOTIFY_NODE_UNLINKED			0x0000003C  // Sent when a node is unlinked (callParam is pointer to node)
#define NOTIFY_NODE_HIDE				0x0000003D  // Sent when a node is hidden (callParam is pointer to node)
#define NOTIFY_NODE_UNHIDE				0x0000003E  // Sent when a node is unhidden (callParam is pointer to node)
#define NOTIFY_NODE_FREEZE				0x0000003F  // Sent when a node is frozen (callParam is pointer to node)
#define NOTIFY_NODE_UNFREEZE			0x00000040  // Sent when a node is unfrozen (callParam is pointer to node)
#define NOTIFY_NODE_PRE_MTL				0x00000041	// Node is about to get a new material (callParam is pointer to node)
#define NOTIFY_NODE_POST_MTL			0x00000042	// Node just got a new material (callParam is pointer to node)
#define NOTIFY_SCENE_ADDED_NODE			0x00000043	// Node just added to scene (callParam is pointer to node)
#define NOTIFY_SCENE_PRE_DELETED_NODE	0x00000044	// Node just removed from scene (callParam is pointer to node)
#define NOTIFY_SCENE_POST_DELETED_NODE	0x00000045	// Node just removed from scene
#define NOTIFY_SEL_NODES_PRE_DELETE		0x00000046	// selected nodes will be deleted. (callParam is pointer to Tab<INode*>)
#define NOTIFY_SEL_NODES_POST_DELETE	0x00000047	// selected nodes just deleted.

#define NOTIFY_WM_ENABLE				0x00000048  // sent when main window gets an WM_ENABLE (BOOL enabled)

#define NOTIFY_SYSTEM_SHUTDOWN			0x00000049	// Max is about to exit,  (system shutdown starting)
#define NOTIFY_SYSTEM_STARTUP			0x00000050	// Max just went live

#define NOTIFY_PLUGIN_LOADED			0x00000051	// A plug-in was just loaded. (callParam is pointer to DllDesc)

#define NOTIFY_SYSTEM_SHUTDOWN2			0x00000052	// Last broadcast before exit. (system shutdown completed)

#define NOTIFY_ANIMATE_ON				0x00000053	// sent when Animate UI mode activated
#define NOTIFY_ANIMATE_OFF				0x00000054	// sent when Animate UI mode de-activated

#define NOTIFY_COLOR_CHANGE				0x00000055	// sent when the system is updating it's custom colors
#define NOTIFY_PRE_EDIT_OBJ_CHANGE  	0x00000056	// sent just before the current edit object is about to chage
#define NOTIFY_POST_EDIT_OBJ_CHANGE  	0x00000057	// sent just afterthe current edit object chages

// NOTE: The radiosity_process notifications are designed to be broadcasted by radiosity plugins (derived from
//       class RadiosityEffect). The broadcast must be implemented in the plugin for the notification to work.
#define NOTIFY_RADIOSITYPROCESS_STARTED	0x00000058	// sent when radiosity processing is started
#define NOTIFY_RADIOSITYPROCESS_STOPPED	0x00000059	// sent when radiosity processing is stopped, but not done
#define NOTIFY_RADIOSITYPROCESS_RESET	0x0000005A	// sent when radiosity processing is reset
#define NOTIFY_RADIOSITYPROCESS_DONE	0x0000005B	// sent when radiosity processing is done

#define NOTIFY_LIGHTING_UNIT_DISPLAY_SYSTEM_CHANGE		0x0000005C	//sent when lighting unit display system is changed

// #define NOTIFY_INSTANCE_CREATED			0x00000053	// sent when a new plugin instance is created via Interface::CreateInstance(), 
												        // callParam is ptr to CreateInstanceCallParam (see below)

// > 10/29/01 - 1:57pm --MQM-- 
// this is helpful for any plugins needing to
// know when we're starting a reflect/refract map,
// or when we're starting the actual frame.
#define NOTIFY_BEGIN_RENDERING_REFLECT_REFRACT_MAP	0x0000005D
#define NOTIFY_BEGIN_RENDERING_ACTUAL_FRAME			0x0000005E
#define NOTIFY_BEGIN_RENDERING_TONEMAPPING_IMAGE	0x0000005F

// [dl | 6mar2002] Broadcast when the radiosity plugin is changed (a new one is assigned)
#define NOTIFY_RADIOSITY_PLUGIN_CHANGED     0x00000060

// [LAM - 3/13/02] Broadcast on scene undo/redo
#define NOTIFY_SCENE_UNDO                   0x00000061
#define NOTIFY_SCENE_REDO                   0x00000062
#define NOTIFY_MANIPULATE_MODE_OFF			0x00000063
#define NOTIFY_MANIPULATE_MODE_ON			0x00000064

// Broadcast when loading XRef (typically surrounding Merge notices)
// 020607  --prs.
#define NOTIFY_SCENE_XREF_PRE_MERGE			0x00000065
#define NOTIFY_SCENE_XREF_POST_MERGE		0x00000066
#define NOTIFY_OBJECT_XREF_PRE_MERGE		0x00000067
#define NOTIFY_OBJECT_XREF_POST_MERGE		0x00000068

// [J.Zhao - 6/10/02] The mirror tool applies to nodes in the selection list
// one at time. Following two notification messages indicates the beginning
// and end of the mirror operations. callParam in the NotifyInfo is a pointer
// to Tab<INode*> consisting of the nodes currently in the selection list
// that the mirror tool is to be applied to.
#define NOTIFY_PRE_MIRROR_NODES				0x00000069
#define NOTIFY_POST_MIRROR_NODES			0x0000006A

// [bayboro | 1may2002] Broadcast on node cloning
#define NOTIFY_NODE_CLONED					0x0000006B // Sent after a node is cloned but before theHold.Accept(..) (callParam is pointer to node)

// [J.Zhao - 10/4/02] The following two notifications may be broadcast
// when NotifyDependents from outside the recursion, that is, not during
// the traversal of reference targets.
// Right now, for examples, the very sources of NotifyDependents() of
//  - REFMSG_MOUSE_CYCLE_STARTED
//  - REFMSG_MOUSE_CYCLE_COMPLETED
// are bracketed by the notifications.
// 
#define NOTIFY_PRE_NOTIFYDEPENDENTS			0x0000006C
#define NOTIFY_POST_NOTIFYDEPENDENTS		0x0000006D

//watje TIMERANGE CALLBACK
//watje time range call back for CS
#define NOTIFY_TIMERANGE_CHANGE				0x0000006E // Sent after the animate time range has been changed


// Start of messages for internal use only.
#define NOTIFY_INTERNAL_USE_START	0x70000000

// Flag values in callParam for NOTIFY_SYSTEM_PRE_NEW:
#define PRE_NEW_NEW_ALL						0x1
#define PRE_NEW_KEEP_OBJECTS				0x2
#define PRE_NEW_KEEP_OBJECTS_AND_HIERARCHY	0x3

// Notification information structure -- Passed to NOTIFYPROC to inform it what
// it's being notified about...
typedef struct {
	int intcode;
	void *callParam;  // this param can be passed in with BroadcastNotification;
	} NotifyInfo;

// The notification callback function
typedef void (* NOTIFYPROC)(void *param, NotifyInfo *info);

// Integer versions -- For pre-defined MAX codes
int CoreExport RegisterNotification(NOTIFYPROC proc, void *param, int code);
int CoreExport UnRegisterNotification(NOTIFYPROC proc, void *param, int code);
void CoreExport BroadcastNotification(int code);
void CoreExport BroadcastNotification(int code, void *callParam);

// Unregister a callback from all codes
int CoreExport UnRegisterNotification(NOTIFYPROC proc, void *param);

class CreateInstanceCallParam {
public:
	void* instance;
	ClassDesc* cd;
	CreateInstanceCallParam (void* i, ClassDesc* cd) : instance(i), cd(cd) { }
}; 

#endif // _NOTIFY_H_
