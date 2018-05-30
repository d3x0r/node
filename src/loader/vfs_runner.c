#include "../../deps/uv/src/sack.h"

#include "memory_dll_loader.h"
#include "app-signature.h"

//#define DEBUG_LIBRARY_LOADING

static struct vfs_runner_local
{
	struct file_system_interface *fsi;
	struct file_system_mounted_interface *rom;
	struct file_system_mounted_interface *ram;
	int(WINAPI*entry_point)(struct volume* (CPROC *load)( CTEXTSTR filepath, uintptr_t version, CTEXTSTR userkey, CTEXTSTR devkey ),void (CPROC*unload)(struct volume *), const char *version );
	struct volume* resource_fs;
	struct volume* core_fs;
	struct volume* user_fs;
	struct volume* user_mirror_fs;
	struct volume* rom_fs;
	struct file_system_mounted_interface *resource_mount;
	struct file_system_mounted_interface *user_mount;
	struct file_system_mounted_interface *user_mirror_mount;
	struct file_system_mounted_interface *core_mount;
}l;


static LOGICAL CPROC LoadLibraryDependant( CTEXTSTR name )
{
	static PLIST loading;
	CTEXTSTR del_string;
#ifdef DEBUG_LIBRARY_LOADING
	lprintf( "LoadLIb %s", name );
#endif
	{
		CTEXTSTR test;
		INDEX idx;
		LIST_FORALL( loading, idx, CTEXTSTR, test )
		{
#ifdef DEBUG_LIBRARY_LOADING
			lprintf( "Compare %s vs %s", name, test );
#endif
			if( StrStr( name, test ) || StrStr( test, name ) )
			{
				//lprintf( "already loading %s(%s)", name, test );
				return TRUE;
			}
		}
	}
	if( IsMappedLibrary( name ) )
		return TRUE;
	AddLink( &loading, del_string = StrDup( name ) );
	if( sack_existsEx( name, l.rom ) )
	{
		FILE *file;
#ifdef DEBUG_LIBRARY_LOADING

		lprintf( "%s exists...", name );
#endif
		file = sack_fopenEx( 0, name, "rb", l.rom );
		if( file )
		{
			CTEXTSTR path = ExpandPath( "*/tmp" );
			TEXTCHAR tmpnam[256];
			TEXTSTR end;
			size_t sz = sack_fsize( file );
			FILE *tmp;
#ifdef DEBUG_LIBRARY_LOADING
			lprintf( "library is %d %s", sz, name );
#endif
			MakePath( path );
			snprintf( tmpnam, 256, "%s/%s", path, name );
			end = pathrchr( tmpnam );
			if( end ) {
				end[0] = 0;
				MakePath( tmpnam );
				end[0] = '/';
			}
			{
				uint32_t startTick = GetTickCount();
				do {
					//lprintf( "Open for output:%s", tmpnam );
					tmp = sack_fopenEx( 0, tmpnam, "wb", sack_get_default_mount() );
					if( !tmp ) {
						lprintf( "Failed to open output file for library... %s", tmpnam );
						WakeableSleep( 250 );
					}
				} while( !tmp && ( ( GetTickCount() - startTick ) < 3000 ) );
			}
#ifdef DEBUG_LIBRARY_LOADING
			lprintf( "Loading %s(%p)", tmpnam, tmp );
#endif
			if( sz && tmp )
			{
				int written, read ;
				POINTER data = NewArray( uint8_t, sz );
				read = sack_fread( data, 1, sz, file );
				written = sack_fwrite( data, 1, sz, tmp );
				sack_fclose( tmp );
#ifdef DEBUG_LIBRARY_LOADING
				lprintf( "written file... closed file...now scanning and then load %d %d", read, written );
#endif
				ScanLoadLibraryFromMemory( name, data, sz, TRUE, LoadLibraryDependant );
				//if( !LoadFunction( name, NULL ) )
				{
					LoadFunction( tmpnam, NULL );
				}
/*
				{
					int n;
					for( n = 0; fixup_entries[n].libname; n++ )
					{
						if( fixup_entries[n].fixed )
							continue;
						if( StrStr( tmpnam, fixup_entries[n].libname ) != 0 )
						{
							POINTER p;
							//lprintf( "need fix abort.." );
							//fixup_entries[n].fixed = TRUE;
							p = LoadLibrary( tmpnam );
							FixAbortLink( p, fixup_entries + n, TRUE );
						}
					}
				}
*/
				//sack_unlinkEx( 0, tmpnam, sack_get_default_mount() );
				Release( data );
			}
			DeleteLink( &loading, del_string );
			sack_fclose( file );
			return TRUE;
		}
	}
	else
		LoadFunction( name, NULL );
    DeleteLink( &loading, del_string );

	return FALSE;
}

static void CPROC InstallFinished( uintptr_t psv, PTASK_INFO task )
{
	((LOGICAL*)psv)[0] = TRUE;

}

PRIORITY_PRELOAD( LowestInit, 1 )
{
		SetProgramName( "program" );
		sack_set_common_data_producer( "Karaway Entertainment" );
}

PRIORITY_PRELOAD( XSaneWinMain, DEFAULT_PRELOAD_PRIORITY + 20 )//( argc, argv )
{
	TEXTSTR cmd = GetCommandLine();
	int argc;
	char **argv;
	//FixupMyTLS();
	//MessageBox( NULL, "Pause", "Attach", MB_OK );
	ParseIntoArgs( cmd, &argc, &argv );
	//SetSystemLog( SYSLOG_FILE, stderr ); 
#ifdef ALT_VFS_NAME
		l.fsi = sack_get_filesystem_interface( "sack_shmem.runner" );
#else
		l.fsi = sack_get_filesystem_interface( "sack_shmem" );
#endif
		//sack_set_default_filesystem_interface( l.fsi );
		SetExternalLoadLibrary( LoadLibraryDependant );
		SetProgramName( "node" );
		sack_set_common_data_producer( "Karaway Entertainment" );
		lprintf( "Open Core FS Log" );

//#define app_signature "FIX ME"
#define _appload( a, b )  sack_vfs_load_crypt_volume( "application.dat",0,#a "-1.0." #b, app_signature )
//#define _appload( a,b )  sack_vfs_load_crypt_volume( "application.dat", a, b )
#define appload( a,b )  _appload( a,b )
#define _appload1( a, b )  sack_vfs_load_crypt_volume( "application.dat",1,#a "-1.0." #b, app_signature )
//#define _appload( a,b )  sack_vfs_load_crypt_volume( "application.dat", a, b )
#define appload1( a,b )  _appload1( a,b )
#define _appload2( a, b )  sack_vfs_load_crypt_volume( "application.dat",2,#a "-1.0." #b, app_signature )
//#define _appload( a,b )  sack_vfs_load_crypt_volume( "application.dat", a, b )
#define appload2( a,b )  _appload2( a,b )

#define _sfxappload( a, b )  sack_vfs_use_crypt_volume( memory, sz,0,#a "-1.0." #b, app_signature )
#define sfxappload( a,b )  _sfxappload( a,b )

#define _resload( a,b )  sack_vfs_load_crypt_volume( "resources.kw",0,#a, "1.0." #b )
#define resload( a,b )  _resload( a,b )

	{
#ifdef STANDALONE_HEADER
		size_t sz = 0;
		POINTER memory;
		{
			uint32_t startTick = GetTickCount();
			do {
				memory = OpenSpace( NULL, argv[0], (uintptr_t*)&sz );
				if( !memory ) {
					lprintf( "Failed to open memory?" );
					WakeableSleep( 250 );
				}
			} while( !memory && ( ( GetTickCount() - startTick ) < 5000 ) );
		}
		//POINTER vfs_memory;
//		lprintf( "memory is %p(%d)\n", memory, sz );
		if( memory == NULL )
		{
			MessageBox( NULL, "Please Launch with full path", "Startup Error", MB_OK );
			exit(0);
		}
		//vfs_memory = GetExtraData( memory );
//		lprintf( "extra is %d(%08x)\n", vfs_memory, vfs_memory );
		l.rom_fs = sfxappload( CMAKE_BUILD_TYPE, CPACK_PACKAGE_VERSION_PATCH );
		//l.rom_fs = appload( CMAKE_BUILD_TYPE, CPACK_PACKAGE_VERSION_PATCH );

//#define l(a,b)		lprintf( "rom is %p %s %s", l.rom_fs, #a "-1.0." #b, app_signature )
//		l( CMAKE_BUILD_TYPE, CMAKE_PACKAGE_VERSION_PATCH );
		if( !l.rom_fs )
			return;
		if( StrCmp( sack_vfs_get_signature( l.rom_fs ), app_signature ) )
			return;
		//printf( "and... we get %p\n", l.rom_fs );
#else
		l.rom_fs = appload( CMAKE_BUILD_TYPE, CPACK_PACKAGE_VERSION_PATCH );
		//printf( "loaded vol:%p\n", l.rom_fs );
		if( !l.rom_fs ) {
			l.rom_fs = appload1( CMAKE_BUILD_TYPE, CPACK_PACKAGE_VERSION_PATCH );
		}
#endif
	}
	{
		uint32_t startTick = GetTickCount();
		do {
			l.rom = sack_mount_filesystem( "self", l.fsi, 900, (uintptr_t)l.rom_fs, FALSE );
			if( !l.rom ) {
				lprintf( "Failed to get rom?" );
				WakeableSleep( 250 );
			}
		} while( !l.rom && ( (GetTickCount() - startTick) < 2000 ) );
	}
	if( 0 )
	{
		TEXTCHAR tmpnam[256];
		PCLIENT pc;
		uint8_t hwaddr[16];
		TEXTCHAR tmpkey[17];
		size_t n, hwlen = 6;
		NetworkStart();
		pc = OpenTCPClient( "www.chatment.com", 80, NULL );
		if( pc && GetMacAddress( pc, hwaddr, &hwlen ) )
			for( n = 0; n < hwlen; n++ )
			{
				snprintf( tmpkey + n * 2, 3, "%02x", hwaddr[n] );
			}
		else
			tmpkey[0] = 0;
		GetModuleFileName( NULL, tmpnam, 256 );
		//lprintf( "Opening default with %s %s", tmpnam, tmpkey );
		{
			int n;
			for( n = 0; n < 3; n++ ) {
				if( ( l.core_fs = sack_vfs_load_crypt_volume( ExpandPath( "*/asset.svfs" ), 0, tmpnam, tmpkey ) ) )
					break;
				if( ( l.core_fs = sack_vfs_load_crypt_volume( ExpandPath( "*/asset.svfs" ), 1, tmpnam, tmpkey ) ) )
					break;
				Sleep( 1000 );
			}
			if( n == 3 ) {
				MessageBox( NULL, "Failed to load application data.", "Failed Initialization", MB_OK );
				exit( 0 );
			}
		}
		RemoveClient( pc );
		l.core_mount = sack_mount_filesystem( "sack_shmem", sack_get_filesystem_interface( "sack_shmem.runner" ), 900, (uintptr_t)l.core_fs, TRUE );
	}

#if 0
	if( !(l.resource_fs = resload( CMAKE_BUILD_TYPE, CPACK_PACKAGE_VERSION_PATCH ) ) )
	{
		//SimpleMessageBox( NULL, "Failed VFS", "Failed to load virtual file system" );
		exit(0);
	}
	l.resource_mount = sack_mount_filesystem( "resource_vfs"
				, l.fsi
				, 950
				, (uintptr_t)l.resource_fs, 0 );
#endif

#ifndef _DEBUG
#  ifdef _WIN32
	if( !LoadFunction( "dl.dll", NULL ) && !LoadFunction( "libdl.dll", NULL ) )
	{
		MessageBox( NULL, "Dynamic Loader support failed", "Load Error", MB_OK );
		exit(0);
	}
#  endif
#endif
	return;
	if( l.rom_fs )
	{
		uint32_t start = GetTickCount();
		do {
#ifdef __GNUC__
#  ifdef WIN32
			l.entry_point = ( int( WINAPI* )( struct volume* ( CPROC *load )( CTEXTSTR filepath, uintptr_t version, CTEXTSTR userkey, CTEXTSTR devkey ), void ( CPROC*unload )( struct volume * ), CTEXTSTR version ) )LoadFunction( "libchatment.standalone.dll", "Start" );
#  else
			l.entry_point = ( int( WINAPI* )( struct volume* ( CPROC *load )( CTEXTSTR filepath, uintptr_t version, CTEXTSTR userkey, CTEXTSTR devkey ), void ( CPROC*unload )( struct volume * ), CTEXTSTR version ) )LoadFunction( "libchatment.standalone.so", "Start" );
#  endif
#else
			l.entry_point = ( int( WINAPI* )( struct volume* ( CPROC *load )( CTEXTSTR filepath, uintptr_t version, CTEXTSTR userkey, CTEXTSTR devkey ), void ( CPROC*unload )( struct volume * ), CTEXTSTR version ) )LoadFunction( "chatment.standalone.dll", "Start" );
#endif
			if( !l.entry_point ) {
				MessageBox( NULL, "STOP HERE AND LOOK", "BLAH", MB_OK );
				lprintf( "Failed to get entry point?" );
				WakeableSleep( 250 );
			}
		} while( !l.entry_point && ( ( GetTickCount() - start ) < 2000 ) );
	}
   //return 0;
}


void premain( void )
{
	InvokeDeadstart();
}


