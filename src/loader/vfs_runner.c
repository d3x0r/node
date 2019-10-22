#include "../../deps/uv/src/sack.h"

#include "memory_dll_loader.h"
#include "app-signature.h"

//#define DEBUG_LIBRARY_LOADING

static struct vfs_runner_local
{
	char** argv;
	struct file_system_interface *fsi;
	struct file_system_mounted_interface *rom;
	struct file_system_mounted_interface *ram;
	int(WINAPI*entry_point)(struct volume* (CPROC *load)( CTEXTSTR filepath, uintptr_t version, CTEXTSTR userkey, CTEXTSTR devkey ),void (CPROC*unload)(struct volume *), const char *version );
	struct sack_vfs_volume* resource_fs;
	struct sack_vfs_volume* core_fs;
	struct sack_vfs_volume* user_fs;
	struct sack_vfs_volume* user_mirror_fs;
	struct sack_vfs_volume* rom_fs;
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
			end = (TEXTSTR)pathrchr( tmpnam );
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

LOGICAL LoadAttachedFreeload( char **argv ) {
	char buf[256];
	size_t sz = 0;
	POINTER memory;
	POINTER freeload;
#ifdef WIN32
#define useName buf
	GetModuleFileName( NULL, buf, 256 );
#else
#define useName argv[0]
#endif
	
	{
		uint32_t startTick = GetTickCount();
		do {
			memory = OpenSpace( NULL, useName, (uintptr_t*)&sz );
			if( !memory ) {
				lprintf( "Failed to open memory?" );
				WakeableSleep( 250 );
			}
		} while( !memory && ( ( GetTickCount() - startTick ) < 5000 ) );
	}
	if( memory == NULL )
	{
		MessageBox( NULL, "Please Launch with full path", useName, MB_OK );
		return FALSE;
	}
	
	freeload = GetExtraData( memory );
//	lprintf( "extra is %d(%08x)\n", vfs_memory, vfs_memory );
	l.rom_fs = sack_vfs_use_crypt_volume( memory, sz,0, NULL, NULL );//((uint8_t*)freeload - 4096), "nodeChecksum?" )

	if( !l.rom_fs ) {
		//printf( "no rom fs; this will abort entry point. %s %s\n", STRSYM(CMAKE_BUILD_TYPE), STRSYM(CPACK_PACKAGE_VERSION_PATCH) );
		return FALSE;
	}
	/*
	if( StrCmp( sack_vfs_get_signature( l.rom_fs ), app_signature ) ) {
		printf( "Signature mismatch; this will abort entry point.\n" );
		return;
	}
	*/
	return TRUE;

}

PRIORITY_PRELOAD( XSaneWinMain, DEFAULT_PRELOAD_PRIORITY + 20 )//( argc, argv )
{
#ifdef ALT_VFS_NAME
	l.fsi = sack_get_filesystem_interface( "sack_shmem.runner" );
#else
	l.fsi = sack_get_filesystem_interface( "sack_shmem" );
#endif
	//sack_set_default_filesystem_interface( l.fsi );
	SetExternalLoadLibrary( LoadLibraryDependant );
	//SetProgramName( "node" );
	sack_set_common_data_producer( "Node.js" );

	//lprintf( "Open Core FS Log" );

#if 0
#  ifdef _WIN32
	if( !LoadLibrary( "application.node" ) ) {
			
	}
#  else
	dlopen( "./application.node", 0 );
#  endif
#endif

	{
		uint32_t startTick = GetTickCount();
		do {
			if( sack_exists( "node.vfs" ) ) l.rom_fs = sack_vfs_load_volume( "node.vfs" );
			if( !l.rom_fs ) if( !LoadAttachedFreeload( l.argv ) ) break;
			if( l.rom_fs ) l.rom = sack_mount_filesystem( "self", l.fsi, 900, (uintptr_t)l.rom_fs, FALSE );
			if( !l.rom ) {
				//lprintf( "Failed to get rom?" );
				WakeableSleep( 250 );
			}
		} while( !l.rom && ( (GetTickCount() - startTick) < 2000 ) );
	}
}

PRIORITY_PRELOAD( UpdateProducer, OSALOT_PRELOAD_PRIORITY - 1 ) {
  sack_set_common_data_producer( "Node.js" );

}


void premain( char** argv )
{
	l.argv = argv;
	InvokeDeadstart();
}


