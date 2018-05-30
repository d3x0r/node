
#include "../../deps/uv/src/sack.h"
#include <TlHelp32.h>

//#define _NTDEF_
//typedef LONG NTSTATUS, *PNTSTATUS;
//#include <Winternl.h>

#include "ntundoc.h"
struct fixup_table_entry {
	CTEXTSTR libname;
	CTEXTSTR import_libname;
	CTEXTSTR import_name;
	POINTER pointer;
	LOGICAL fixed;
};


static void FakeAbort( void );


static void WINAPI CreateFileA_stub( int args ) {
	lprintf( "create fileA called" );
}
static void WINAPI CreateFileW_stub( int args ) {
	lprintf( "create fileA called" );
}

static struct fixup_table_entry fixup_entries[] = { { "libgcc_s_dw2-1.dll", "msvcrt.dll", "abort", (POINTER)FakeAbort } 
					, { "kernel32.dll", "kernel32.dll", "CreateFileA", CreateFileA_stub }
					, { "kernel32.dll", "kernel32.dll", "CreateFileW", CreateFileW_stub }
					, {NULL }
					};


static void FakeAbort( void )
{
	TerminateProcess( GetCurrentProcess(), 0 );
}

static void *MyAlloc( size_t size )
{
	return Allocate( size );
}

static void MyFree( void *p )
{
	Deallocate( void *, p );
}
static void * MyRealloc( void *p, size_t size )
{
	return Reallocate( p, size );
}

#define Seek(a,b) (((uintptr_t)a)+(b))

typedef struct task_info_tag *PTASK_INFO;

typedef struct handle_info_tag
{
	//struct mydatapath_tag *pdp;
	// partial inputs...
	PTEXT pLine;
	char *name;
	int       bNextNew;
	PTHREAD   hThread;
#ifdef WIN32
	// read/write handle
	HANDLE    handle;
#else
	int       pair[2];
	// read/write handle
	int       handle;
#endif
} HANDLEINFO, *PHANDLEINFO;

struct task_info_tag {
	struct {
		BIT_FIELD closed : 1;
		BIT_FIELD process_ended : 1;
		BIT_FIELD bSentIoTerminator : 1;
		BIT_FIELD log_input : 1;
	} flags;
	TaskEnd EndNotice;
	TaskOutput OutputEvent;
	uintptr_t psvEnd;
	HANDLEINFO hStdIn;
	HANDLEINFO hStdOut;
	volatile PTHREAD pOutputThread;
	//HANDLEINFO hStdErr;
#if defined(WIN32)
	HANDLE hReadOut, hWriteOut;
	//HANDLE hReadErr, hWriteErr;
	HANDLE hReadIn, hWriteIn;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD exitcode;
#elif defined( __LINUX__ )
	int hReadOut, hWriteOut;
	//HANDLE hReadErr, hWriteErr;
	int hReadIn, hWriteIn;
	pid_t pid;
	uint32_t exitcode;
#endif
};

#if asdfasdf
typedef struct _RTL_USER_PROCESS_PARAMETERS {

	ULONG                   MaximumLength;
	ULONG                   Length;
	ULONG                   Flags;
	ULONG                   DebugFlags;
	PVOID                   ConsoleHandle;
	ULONG                   ConsoleFlags;
	HANDLE                  StdInputHandle;
	HANDLE                  StdOutputHandle;
	HANDLE                  StdErrorHandle;
	UNICODE_STRING          CurrentDirectoryPath;
	HANDLE                  CurrentDirectoryHandle;
	UNICODE_STRING          DllPath;
	UNICODE_STRING          ImagePathName;
	UNICODE_STRING          CommandLine;
	PVOID                   Environment;
	ULONG                   StartingPositionLeft;
	ULONG                   StartingPositionTop;
	ULONG                   Width;
	ULONG                   Height;
	ULONG                   CharWidth;
	ULONG                   CharHeight;
	ULONG                   ConsoleTextAttributes;
	ULONG                   WindowFlags;
	ULONG                   ShowWindowFlags;
	UNICODE_STRING          WindowTitle;
	UNICODE_STRING          DesktopName;
	UNICODE_STRING          ShellInfo;
	UNICODE_STRING          RuntimeData;
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];

} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_LDR_DATA {




	ULONG                   Length;
	BOOLEAN                 Initialized;
	PVOID                   SsHandle;
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;



} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {



	BOOLEAN                 InheritedAddressSpace;
	BOOLEAN                 ReadImageFileExecOptions;
	BOOLEAN                 BeingDebugged;
	BOOLEAN                 Spare;
	HANDLE                  Mutant;
	PVOID                   ImageBaseAddress;
	PPEB_LDR_DATA           LoaderData;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID                   SubSystemData;
	PVOID                   ProcessHeap;
	PVOID                   FastPebLock;
	PPEBLOCKROUTINE         FastPebLockRoutine;
	PPEBLOCKROUTINE         FastPebUnlockRoutine;
	ULONG                   EnvironmentUpdateCount;
	PPVOID                  KernelCallbackTable;
	PVOID                   EventLogSection;
	PVOID                   EventLog;
	PPEB_FREE_BLOCK         FreeList;
	ULONG                   TlsExpansionCounter;
	PVOID                   TlsBitmap;
	ULONG                   TlsBitmapBits[0x2];
	PVOID                   ReadOnlySharedMemoryBase;
	PVOID                   ReadOnlySharedMemoryHeap;
	PPVOID                  ReadOnlyStaticServerData;
	PVOID                   AnsiCodePageData;
	PVOID                   OemCodePageData;
	PVOID                   UnicodeCaseTableData;
	ULONG                   NumberOfProcessors;
	ULONG                   NtGlobalFlag;
	BYTE                    Spare2[0x4];
	LARGE_INTEGER           CriticalSectionTimeout;
	ULONG                   HeapSegmentReserve;
	ULONG                   HeapSegmentCommit;
	ULONG                   HeapDeCommitTotalFreeThreshold;
	ULONG                   HeapDeCommitFreeBlockThreshold;
	ULONG                   NumberOfHeaps;
	ULONG                   MaximumNumberOfHeaps;
	PPVOID                  *ProcessHeaps;
	PVOID                   GdiSharedHandleTable;
	PVOID                   ProcessStarterHelper;
	PVOID                   GdiDCAttributeList;
	PVOID                   LoaderLock;
	ULONG                   OSMajorVersion;
	ULONG                   OSMinorVersion;
	ULONG                   OSBuildNumber;
	ULONG                   OSPlatformId;
	ULONG                   ImageSubSystem;
	ULONG                   ImageSubSystemMajorVersion;
	ULONG                   ImageSubSystemMinorVersion;
	ULONG                   GdiHandleBuffer[0x22];
	ULONG                   PostProcessInitRoutine;
	ULONG                   TlsExpansionBitmap;
	BYTE                    TlsExpansionBitmapBits[0x80];
	ULONG                   SessionId;

} PEB, *PPEB;
#endif

#ifndef _WINTERNL_
typedef struct _LDR_DATA_TABLE_ENTRY {
	struct _LDR_DATA_TABLE_ENTRY *fore;
	struct _LDR_DATA_TABLE_ENTRY *back;
	LIST_ENTRY InMemoryOrderLinks;
	PVOID Reserved2[2];
	PVOID DllBase;
	PVOID EntryPoint;
	PVOID Reserved3;
	UNICODE_STRING FullDllName;
	BYTE Reserved4[8];
	PVOID Reserved5[3];
	union {
		ULONG CheckSum;
		PVOID Reserved6;
	};
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;
#else
#define LoaderData Ldr
#define ImageBaseAddress Reserved3[1]
#endif

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation = 0,
	ProcessWow64Information = 26
} PROCESSINFOCLASS;

//#include <ntddk.h>
extern NTSTATUS( NTAPI *pNtQueryInformationProcess )(
	HANDLE ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength);


 void FixLink( PTASK_INFO task, POINTER p )
{
	struct fixup_table_entry *entry = fixup_entries;
	LOGICAL gcclib = FALSE;
	POINTER source_memory = p;
	POINTER real_memory = p;
	//printf( "Load %s (%d:%d)\n", name, generation, level );
	{

		{
			IMAGE_DOS_HEADER source_dos_header;
			//PIMAGE_NT_HEADERS source_nt_header;
			SIZE_T nRead;
			PROCESS_INFORMATION processInformation = { 0 };
			PPEB peb = (PPEB)p;
			MODULEENTRY32 modent32;
			PLDR_DATA_TABLE_ENTRY ldte;
			//MODULEENTRY modent64;
			BOOL r;
			HANDLE modules = CreateToolhelp32Snapshot(
				TH32CS_SNAPMODULE,
				task->pi.dwProcessId
			);

			ldte = (PLDR_DATA_TABLE_ENTRY)peb->LoaderData->InLoadOrderModuleList.Flink;

//	TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
			if( modules == INVALID_HANDLE_VALUE ) {
				DWORD err = GetLastError();
				lprintf( "return %d", err );
				return;
			}
			for( r = Module32First( modules, &modent32 );
				r;
				r = Module32Next( modules, &modent32 ) ) {
				if( !ReadProcessMemory( task->pi.hProcess, (void*)modent32.modBaseAddr, (void*)&source_dos_header, sizeof( source_dos_header ), &nRead ) ) {
					lprintf( "Read failed." );
					return;
				}
			}
//	NtQueryInformationProcess( task->pi.hProcess, &processInformation );
			if( !ReadProcessMemory( task->pi.hProcess, (const void*)peb->ImageBaseAddress, (void*)&source_dos_header, sizeof( source_dos_header ), &nRead ) ) {
				lprintf( "Read failed." );
				return;
			}
			LogBinary((uint8_t*) &source_dos_header, sizeof( source_dos_header ));
			if( source_dos_header.e_magic != IMAGE_DOS_SIGNATURE ) {
			 lprintf( "Basic signature check failed; not a library" );
			 return ;
			}
 /*
	if( source_nt_header->Signature != IMAGE_NT_SIGNATURE ) {
	 LoG( "Basic NT signature check failed; not a library" );
	 return NULL;
	}
	if( source_nt_header->FileHeader.SizeOfOptionalHeader )
	{
	 if( source_nt_header->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC )
	 {
	  LoG( "Optional header signature is incorrect..." );
	  return NULL;
	 }
	}
	{
	 int n;
	 long FPISections = source_dos_header->e_lfanew
	  + sizeof( DWORD ) + sizeof( IMAGE_FILE_HEADER )
	  + source_nt_header->FileHeader.SizeOfOptionalHeader;
	 PIMAGE_SECTION_HEADER source_section = (PIMAGE_SECTION_HEADER)Seek( source_memory, FPISections );
	 uintptr_t dwSize = 0;
	 uintptr_t newSize;
	 source_section = (PIMAGE_SECTION_HEADER)Seek( source_memory, FPISections );
	 for( n = 0; n < source_nt_header->FileHeader.NumberOfSections; n++ )
	 {
	  newSize = (source_section[n].PointerToRawData) + source_section[n].SizeOfRawData;
	  if( newSize > dwSize )
		dwSize = newSize;
	 }
	 dwSize += (BLOCK_SIZE*2)-1; // pad 1 full block, plus all but 1 byte of a full block(round up)
	 dwSize &= ~(BLOCK_SIZE-1); // mask off the low bits; floor result to block boundary
	 return (POINTER)Seek( source_memory, dwSize );
	 }
		 */

#if 0

		PIMAGE_DOS_HEADER source_dos_header = (PIMAGE_DOS_HEADER)source_memory;
		PIMAGE_NT_HEADERS source_nt_header = (PIMAGE_NT_HEADERS)Seek( source_memory, source_dos_header->e_lfanew );
		if( source_dos_header->e_magic != IMAGE_DOS_SIGNATURE )
		{
			lprintf( "Basic signature check failed; not a library" );
			return;
		}

		if( source_nt_header->Signature != IMAGE_NT_SIGNATURE )
		{
			lprintf( "Basic NT signature check failed; not a library" );
			return;
		}

		if( source_nt_header->FileHeader.SizeOfOptionalHeader )
		{
			if( source_nt_header->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC )
			{
				//lprintf( "Optional header signature is incorrect... %x = %x",source_nt_header->OptionalHeader.Magic, IMAGE_NT_OPTIONAL_HDR_MAGIC );
			}
		}
		{
			//int n;
			PIMAGE_DATA_DIRECTORY dir = (PIMAGE_DATA_DIRECTORY)source_nt_header->OptionalHeader.DataDirectory;
			long FPISections = source_dos_header->e_lfanew
				+ sizeof( DWORD ) + sizeof( IMAGE_FILE_HEADER )
				+ source_nt_header->FileHeader.SizeOfOptionalHeader;
			PIMAGE_SECTION_HEADER source_section;
			PIMAGE_IMPORT_DESCRIPTOR real_import_base;
			PIMAGE_SECTION_HEADER source_import_section = NULL;
			PIMAGE_SECTION_HEADER source_text_section = NULL;
			uintptr_t dwSize = 0;
			//uintptr_t newSize;
			source_section = (PIMAGE_SECTION_HEADER)Seek( source_memory, FPISections );
			// compute size of total of sections
			// mark a few known sections for later processing
			// ------------- Go through the sections and move to expected virtual offsets
			real_import_base = (PIMAGE_IMPORT_DESCRIPTOR)Seek( real_memory, dir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
			if( real_import_base &&  dir[IMAGE_DIRECTORY_ENTRY_IMPORT].Size )
			{
				int n;
				for( n = 0; real_import_base[n].Characteristics; n++ )
				{
					const char * dll_name;
					int f;
					uintptr_t *dwFunc;
					uintptr_t *dwTargetFunc;
					PIMAGE_IMPORT_BY_NAME import_desc;
					if( real_import_base[n].Name )
						dll_name = (const char*) Seek( real_import_base, real_import_base[n].Name - dir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress/*source_import_section->VirtualAddress*/ );
					if( gcclib )
					{
						if( StrCmp( dll_name, entry->import_libname ) )
						{
							//lprintf( "skipping %s", dll_name );
							continue;
						}
					}
					lprintf( "import %s", dll_name );
					//char * function_name = (char*) Seek( import_base, import_base[n]. - source_import_section->VirtualAddress );
					//printf( "thing %s\n", dll_name );
#if __WATCOMC__ && __WATCOMC__ < 1200
					dwFunc = (uintptr_t*)Seek( real_import_base, real_import_base[n].OrdinalFirstThunk - dir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
#else
					dwFunc = (uintptr_t*)Seek( real_import_base, real_import_base[n].OriginalFirstThunk - dir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
#endif
					dwTargetFunc = (uintptr_t*)Seek( real_import_base, real_import_base[n].FirstThunk - dir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
					for( f = 0; dwFunc[f]; f++ )
					{
						if( dwFunc[f] & ( (uintptr_t)1 << ( ( sizeof( uintptr_t ) * 8 ) - 1 ) ) )
						{
							lprintf( "Oridinal is %d", dwFunc[f] & 0xFFFF );
							//dwTargetFunc[f] = (uintptr_t)LoadFunction( dll_name, (CTEXTSTR)(dwFunc[f] & 0xFFFF) );
						}
						else
						{
							import_desc = (PIMAGE_IMPORT_BY_NAME)Seek( real_memory, dwFunc[f] );
							lprintf( " sub thing %s", import_desc->Name );
							if( gcclib )
							{
								if( StrCmp( (CTEXTSTR)import_desc->Name, entry->import_name ) == 0 )
								{
									//lprintf( "Fixed abort...%p to %p", dwTargetFunc + f, entry->pointer );
									dwTargetFunc[f] = (uintptr_t)entry->pointer;
									return;
								}
							}
#if 0
							else
							{
			{
				DWORD old_protect;
				VirtualProtect( (POINTER)( dwTargetFunc + f )
						, 4
						, PAGE_EXECUTE_READWRITE
						, &old_protect );
								if( StrCmp( (CTEXTSTR)import_desc->Name, "malloc" ) == 0 )
								{
									dwTargetFunc[f] = (uintptr_t)MyAlloc;
									//return;
								}
								if( StrCmp( (CTEXTSTR)import_desc->Name, "free" ) == 0 )
								{
									dwTargetFunc[f] = (uintptr_t)MyFree;
									//return;
								}
								if( StrCmp( (CTEXTSTR)import_desc->Name, "realloc" ) == 0 )
								{
									dwTargetFunc[f] = (uintptr_t)MyRealloc;
									//return;
								}

				VirtualProtect( (POINTER)( dwTargetFunc + f )
						, 4
						, old_protect
						, &old_protect );
			}
							}
#endif
						}
					}
				}
			}
#endif
		}
	}
}

