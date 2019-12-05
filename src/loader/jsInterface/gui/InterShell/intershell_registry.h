#ifndef INTERSHELL_REGISTRY_SHORTCUTS_DEFINED
#define INTERSHELL_REGISTRY_SHORTCUTS_DEFINED

#define InterShell_REGISTRY // symbol to test if this was included....
#define TASK_PREFIX "intershell"

#ifdef SACK_CORE_BUILD
#ifndef INTERSHELL_CORE_BUILD
#define INTERSHELL_CORE_BUILD
#endif
#endif
#if defined( INTERSHELL_CORE_BUILD ) || defined( LEGACY_MAKE_SYSTEM )
#include "widgets/include/buttons.h"
#else
#include "widgets/buttons.h"
#endif
#ifndef POSTFIX
#define POSTFIX ""
#endif

#ifdef __cplusplus
namespace sack {
	namespace intershell {
#endif

#ifndef MENU_BUTTON_DEFINED
#define MENU_BUTTON_DEFINED
		/* Pointer to the basic control that InterShell uses to track
		   controls on pages. This is given to a plugin when a control
		   is created, and the control should just treat this as a
		   handle and use proper interface methods with intershell.core
		   to get information.                                          */
		typedef struct menu_button *PMENU_BUTTON;
#endif

// static uintptr_t OnCreateMenuButton("name")(PMENU_BUTTON button ) { /*create button data*/ }
#define OnCreateMenuButton(name) \
	DefineRegistryMethod(TASK_PREFIX,CreateMenuButton,"control",name POSTFIX,"button_create",uintptr_t,(PMENU_BUTTON),__LINE__)

// parametrs to this are the parent control, x, y, width and height
// static uintptr_t OnCreateControl("")(PSI_CONTROL parent,int32_t x,int32_t y,uint32_t w,uint32_t h)
#define OnCreateControl(name) \
	DefineRegistryMethod(TASK_PREFIX,CreateControl,"control",name,"control_create",uintptr_t,(PSI_CONTROL,int32_t,int32_t,uint32_t,uint32_t),__LINE__)
#define OnCreateListbox(name) \
	DefineRegistryMethod(TASK_PREFIX,CreateMenuListbox,"control",name,"listbox_create",uintptr_t,(PSI_CONTROL),__LINE__)

#define OnDestroyMenuButton(name) \
	  DefineRegistryMethod(TASK_PREFIX,DestroyMenuButton,"control",name,"button_destroy",void,(uintptr_t),__LINE__)
#define OnDestroyControl OnDestroyMenuButton
#define OnDestroyListbox OnDestroyMenuButton

// Return the real PSI_CONTROL; which may not be the uintptr_t result from before
// static PSI_CONTROL OnGetControl("")(uintptr_t psvInit);
// { return (PSI_CONTROL)psvInit; }
#define OnGetControl(name) \
	DefineRegistryMethod(TASK_PREFIX,GetShellControl,"control",name,"get_control",PSI_CONTROL,(uintptr_t),__LINE__)
#define OnFixupControl(name) \
	  DefineRegistryMethod(TASK_PREFIX,FixupControl,"control",name,"control_fixup",void,(uintptr_t),__LINE__)
#define OnFixupMenuButton OnFixupControl

// things like clock can do a clock unlock
// things like lists can requery databases to show new items....
#define OnShowControl(name) \
	DefineRegistryMethod(TASK_PREFIX,ShowControl,"control",name,"show_control",void,(uintptr_t),__LINE__)
// static void OnHideControl( "" )( uintptr_t psv )
#define OnHideControl(name) \
	DefineRegistryMethod(TASK_PREFIX,HideControl,"control",name,"hide_control",void,(uintptr_t),__LINE__)

// static void OnLoadControl( "")( PCONFIG_HANDLER, uintptr_t psv )
#define OnLoadControl( name ) \
     DefineRegistryMethod(TASK_PREFIX,LoadControl,"control",name,"control_config",void,(PCONFIG_HANDLER,uintptr_t),__LINE__)
#define OnLoad( name ) \
     _DefineRegistryMethod(TASK_PREFIX,LoadControl,"control",name,"control_config",void,(XML_Parser,uintptr_t),__LINE__)

#define OnConfigureControl(name) \
	DefineRegistryMethod(TASK_PREFIX,ConfigureControl,"control",name,"control_edit",uintptr_t,(uintptr_t,PSI_CONTROL),__LINE__)
#define OnEditControl OnConfigureControl

#define OnEditBegin(name) \
	  DefineRegistryMethod(TASK_PREFIX,EditBegin,"control",name,"on_menu_edit_begin",void,(uintptr_t),__LINE__)
#define OnEditEnd(name) \
	  DefineRegistryMethod(TASK_PREFIX,EditEnd,"control",name,"on_menu_edit_end",void,(uintptr_t),__LINE__)

#define OnEditModeBegin(name) \
	  DefineRegistryMethod(TASK_PREFIX,EditBegin,"common","Begin Edit Mode", name "_on_menu_edit_begin",void,(void),__LINE__)
#define OnEditModeEnd(name) \
	  DefineRegistryMethod(TASK_PREFIX,EditEnd,"common","End Edit Mode",name "_on_menu_edit_end",void,(void),__LINE__)

#define OnSelectListboxItem(name,othername) \
	DefineRegistrySubMethod(TASK_PREFIX,ListSelectionChanged,"control",name,"listbox_selection_changed",othername,void,(uintptr_t,PLISTITEM))
#define OnDoubleSelectListboxItem(name,othername) \
	DefineRegistrySubMethod(TASK_PREFIX,ListDoubleSelectionChanged,"control",name,"listbox_double_changed",othername,void,(uintptr_t,PLISTITEM))

/* 
 * this is depreicated, buttons shouldn't really HAVE to know the button they are... 
 *
 */
//#define OnSaveMenuButton(name)
//	  DefineRegistryMethod(TASK_PREFIX,SaveButton,"control",name,"button_save",void,(FILE*,PMENU_BUTTON,uintptr_t))
//#define OnSaveCommon OnSaveMenuButton
#define OnSave(name) \
	  _DefineRegistryMethod(TASK_PREFIX,SaveButton,"control",name,"control_save_xml",void,(genxWriter,uintptr_t),__LINE__)

#define OnSaveControl(name) \
	  DefineRegistryMethod(TASK_PREFIX,SaveButton,"control",name,"control_save",void,(FILE*,uintptr_t),__LINE__)

#define OnSaveControl(name) \
	DefineRegistryMethod(TASK_PREFIX,SaveButton,"control",name,"control_save",void,(FILE*,uintptr_t),__LINE__)
/* this method is depricated also, and will soon be obsolete.... perfer usage of OnSave and OnLoad for further development */
#define OnSaveMenuButton(name) \
	  DefineRegistryMethod(TASK_PREFIX,SaveButton,"control",name,"button_save",void,(FILE*,PMENU_BUTTON,uintptr_t),__LINE__)

// return TRUE/FALSE whether the control should be shown, else it remains hidden.
// this method is used both with MenuButton and Control.
// This is called after FixupButton(on MenuButtonOnly) and EndEdit( on Controls and MenuButtons )
// this is called during RestorePage(Full), which is used by ChangePage() after HidePage().
#define OnQueryShowControl( name )  \
	  DefineRegistryMethod(TASK_PREFIX,QueryShowControl,"control",name,"query can show",LOGICAL,(uintptr_t),__LINE__)

// static int OnChangePage("")( PSI_CONTROL pc_canvas )
#define OnChangePage(name) \
	  DefineRegistryMethod(TASK_PREFIX,ChangePage,"common","change page",name "_on_change_page",int,(PSI_CONTROL),__LINE__)

// static void OnLoadCommon("")(PCONFIG_HANDLER pch )
// can use InterShell_GetCurrentLoadingCanvas() to get the current canvas being loaded
#define OnLoadCommon( name )  \
	  DefineRegistryMethod(TASK_PREFIX,LoadCommon,"common","common_config",name "_on_load",void,(PCONFIG_HANDLER),__LINE__)

// static void OnSaveCommon("")(FILE*file_out )
// can use InterShell_GetCurrentSavingCanvas() to get the current canvas being loaded
#define OnSaveCommon(name) \
	   DefineRegistryMethod(TASK_PREFIX,SaveCommon,"common","save common",name "_on_save_common",void,(FILE*),__LINE__)

// handler to catch event of when a page is saved.  The currnet ppage_data is passed as a convenience
#define OnSavePage(name) \
	   DefineRegistryMethod(TASK_PREFIX,SavePage,"common","save page",name "_on_save_page",void,(FILE*,PPAGE_DATA),__LINE__)
#define OnSaveXMLPage(name) \
	   DefineRegistryMethod(TASK_PREFIX,SavePage,"common","save xml page",name "_on_save_page",void,(genxWriter,PPAGE_DATA),__LINE__)

// invoked when all other initization is done, and before the main applciation actually runs and displays stuff.
//
// static void OnFinishInit("")(PSI_CONTROL pc_canvas);
#define OnFinishInit( name ) \
      DefineRegistryMethod(TASK_PREFIX,FinishInit,"common","finish init",name "_on_finish_init",void,(PSI_CONTROL),__LINE__)

// invoked when ALL initialzation is run, just after the menu is shown.  (tasks, first load)
#define OnFinishAllInit( name ) \
      DefineRegistryMethod(TASK_PREFIX,FinishAllInit,"common","finish all init",name "_on_finish_all_init",void,(void),__LINE__)

// invoked when ALL initialzation is run, just after the menu is shown.  (tasks, first load)
#define OnTaskLaunchComplete( name ) \
      DefineRegistryMethod(TASK_PREFIX,TaskLaunchComplete,"common","task launch complete",name "_on_task_launch_complete",void,(void),__LINE__)

// passed the PSI_CONTROL of the menu canvas, used to give the control the parent to display its frame against.
// may also be used to get the current page.
#define OnGlobalPropertyEdit( name ) \
      DefineRegistryMethod(TASK_PREFIX,GlobalProperty,"common","global properties",name,void,(PSI_CONTROL),__LINE__)

#define OnInterShellShutdown( name ) \
      DefineRegistryMethod(TASK_PREFIX,InterShellShutdown,"common","intershell shutdown",name "_on_intershell_shutdown",void,(void),__LINE__)

// static LOGICAL OnDropAccept("")(PSI_CONTROL pc_canvas,CTEXTSTR filepath,int x,int y)
#define OnDropAccept(name) \
	DefineRegistryMethod(TASK_PREFIX,DropAccept,"common","Drop Accept",name "_on_drop_accept",LOGICAL,(PSI_CONTROL,CTEXTSTR,int,int),__LINE__)

//GETALL_REGISTERED( "issue_pos/common/common_config" )

#define GETALL_REGISTERED( root,type,args )	{          \
		CTEXTSTR name;                           \
		PCLASSROOT data = NULL;                     \
		for( name = GetFirstRegisteredName( root, &data );  \
			 name;                                           \
			  name = GetNextRegisteredName( &data ) )        \
		{ type (CPROC *f)args; f = GetRegisteredProcedure2(root,type,name,args);

#define ENDALL_REGISTERED()		} }


//-------------
// second generation (post documentation)

// Method invoked when a control is selected and a copy operation invoked.
// New method requires 'static &lt;return type&gt;' to be applied...
// static void OnCopyControl( "blah" )( uintptr_t psvYourControl );
#define OnCopyControl(name) \
	DefineRegistryMethod(TASK_PREFIX,CopyControl,"control",name,"copy_control",void,(uintptr_t),__LINE__)

// Method invoked when a control is selected and a paste operation invoked.
// New method requires 'static &lt;return type&gt;' to be applied...
// static void OnPasteControl( "blah" )( uintptr_t psvYourControl );
#define OnPasteControl(name) \
	DefineRegistryMethod(TASK_PREFIX,PasteControl,"control",name,"paste_control",void,(uintptr_t),__LINE__)

// Event handler invoked when a control is cloned (using existing interface)
// static void OnCloneControl( "blah" )( uintptr_t psvControlThatIsNewClone, uintptr_t psvControlThatIsBeingCloned )
#define OnCloneControl(name) \
	DefineRegistryMethod(TASK_PREFIX,CloneControl,"control",name,"clone_control",void,(uintptr_t,uintptr_t),__LINE__)

#define OnLoadSecurityContext( name ) \
	DefineRegistryMethod(TASK_PREFIX,LoadSecurityContext,"common/security","Load Security",name "_load_security",void,(PCONFIG_HANDLER),__LINE__)

#define OnSaveSecurityContext( name ) \
	DefineRegistryMethod(TASK_PREFIX,SaveSecurityContext,"common/security","Save Security",name "_save_security",void,(FILE*,uintptr_t),__LINE__)

#define OnAddSecurityContextToken( name ) \
	DefineRegistryMethod(TASK_PREFIX,AddSecurityContextToken_,"common/security/Add Security Token",name, "add_token",void,(uintptr_t,CTEXTSTR),__LINE__)

#define OnGetSecurityContextTokens( name ) \
	DefineRegistryMethod(TASK_PREFIX,GetSecurityContextToken_,"common/security/Get Security Tokens",name, "get_tokens",void,(uintptr_t,PLIST*),__LINE__)

/* result INVALID_INDEX - premission denied
 result 0 - no context
 any other result is a result handle that is also closed when complete */
#define TestSecurityContext( name ) \
	DefineRegistryMethod(TASK_PREFIX,TestSecurityContext,"common/security","Test Security",name "_test_security",uintptr_t,(uintptr_t),__LINE__)
#define EndSecurityContext( name ) \
	DefineRegistryMethod(TASK_PREFIX,TestSecurityContext,"common/security","Close Security",name "_close_security",void,(uintptr_t,uintptr_t),__LINE__)

#define OnEditSecurityContext( name ) \
	DefineRegistryMethod(TASK_PREFIX,EditSecurityContext,"common/security","Edit Security",name,void,(uintptr_t),__LINE__)

/* Intended use:edits properties regarding page security... OnPageChange return FALSE to disallow page change...*/
#define OnEditPageSecurityContext( name ) \
	DefineRegistryMethod(TASK_PREFIX,EditSecurityContext,"common","global properties","Page Security:"name,void,(PPAGE_DATA),__LINE__)

#define OnApplicationQuit( name ) \
	DefineRegistryMethod(TASK_PREFIX,IntershellQuitApplication,"common","quit application",name,void,(void),__LINE__)

/* Intended use: Supply configuration slots for theme_id
 static void OnThemeAdded( name )( int theme_id )
 */
#define OnThemeAdded( name ) \
	DefineRegistryMethod(TASK_PREFIX,ThemeAdded,"common","theme/add","Theme Add:"name,void,(int theme_id),__LINE__)

/* Intended use: Theme is changing, the theme_id that is given was the prior theme set
 static void OnThemeChanging( name )( int theme_id )
*/
#define OnThemeChanging( name ) \
	DefineRegistryMethod(TASK_PREFIX,ThemeChanging,"common","theme/changing","Theme Changing:"name,void,(int theme_id),__LINE__)

/* Intended use: Receive event that theme has changed, use theme_id to update graphics if available
 static void OnThemeChanging( name )( int theme_id )
*/
#define OnThemeChanged( name ) \
	DefineRegistryMethod(TASK_PREFIX,ThemeChanged,"common","theme/changed","Theme Change:"name,void,(int theme_id),__LINE__)

#ifdef __cplusplus
	} }
using namespace sack::intershell;
#endif

#endif

