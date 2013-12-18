# Microsoft Developer Studio Generated NMAKE File, Based on SCT Draw Activex.dsp
!IF "$(CFG)" == ""
CFG=SCT Draw Activex - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SCT Draw Activex - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SCT Draw Activex - Win32 Release" && "$(CFG)" != "SCT Draw Activex - Win32 Debug" && "$(CFG)" != "SCT Draw Activex - Win32 Unicode Debug" && "$(CFG)" != "SCT Draw Activex - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCT Draw Activex.mak" CFG="SCT Draw Activex - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCT Draw Activex - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCT Draw Activex - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCT Draw Activex - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCT Draw Activex - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SCT Draw Activex.ocx" "$(OUTDIR)\SCT Draw Activex.pch" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\ArcUnit.obj"
	-@erase "$(INTDIR)\BaseUnit.obj"
	-@erase "$(INTDIR)\BitmapUnit.obj"
	-@erase "$(INTDIR)\Combination.obj"
	-@erase "$(INTDIR)\DisjointUnit.obj"
	-@erase "$(INTDIR)\EllipseUnit.obj"
	-@erase "$(INTDIR)\EraseUnit.obj"
	-@erase "$(INTDIR)\LineUnit.obj"
	-@erase "$(INTDIR)\PenBrushUnit.obj"
	-@erase "$(INTDIR)\PolygonUnit.obj"
	-@erase "$(INTDIR)\PolylineUnit.obj"
	-@erase "$(INTDIR)\RectUnit.obj"
	-@erase "$(INTDIR)\RgnObjUnit.obj"
	-@erase "$(INTDIR)\RoundRectUnit.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.pch"
	-@erase "$(INTDIR)\SCT Draw Activex.res"
	-@erase "$(INTDIR)\SCT Draw Activex.tlb"
	-@erase "$(INTDIR)\SCTDrawActivexCtl.obj"
	-@erase "$(INTDIR)\SCTDrawActivexPpg.obj"
	-@erase "$(INTDIR)\SectorUnit.obj"
	-@erase "$(INTDIR)\SplitUnit.obj"
	-@erase "$(INTDIR)\SprayBottleUnit.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StylesDlg.obj"
	-@erase "$(INTDIR)\TextUnit.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SCT Draw Activex.exp"
	-@erase "$(OUTDIR)\SCT Draw Activex.lib"
	-@erase "$(OUTDIR)\SCT Draw Activex.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SCT Draw Activex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=zlib.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SCT Draw Activex.pdb" /machine:I386 /def:".\SCT Draw Activex.def" /out:"$(OUTDIR)\SCT Draw Activex.ocx" /implib:"$(OUTDIR)\SCT Draw Activex.lib" 
DEF_FILE= \
	".\SCT Draw Activex.def"
LINK32_OBJS= \
	"$(INTDIR)\ArcUnit.obj" \
	"$(INTDIR)\BaseUnit.obj" \
	"$(INTDIR)\BitmapUnit.obj" \
	"$(INTDIR)\Combination.obj" \
	"$(INTDIR)\DisjointUnit.obj" \
	"$(INTDIR)\EllipseUnit.obj" \
	"$(INTDIR)\EraseUnit.obj" \
	"$(INTDIR)\LineUnit.obj" \
	"$(INTDIR)\PenBrushUnit.obj" \
	"$(INTDIR)\PolygonUnit.obj" \
	"$(INTDIR)\PolylineUnit.obj" \
	"$(INTDIR)\RectUnit.obj" \
	"$(INTDIR)\RgnObjUnit.obj" \
	"$(INTDIR)\RoundRectUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.obj" \
	"$(INTDIR)\SCTDrawActivexCtl.obj" \
	"$(INTDIR)\SCTDrawActivexPpg.obj" \
	"$(INTDIR)\SectorUnit.obj" \
	"$(INTDIR)\SplitUnit.obj" \
	"$(INTDIR)\SprayBottleUnit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StylesDlg.obj" \
	"$(INTDIR)\TextUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.res"

"$(OUTDIR)\SCT Draw Activex.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\SCT Draw Activex.ocx
InputPath=.\Release\SCT Draw Activex.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SCT Draw Activex.ocx" "$(OUTDIR)\SCT Draw Activex.pch" "$(OUTDIR)\SCT Draw Activex.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\ArcUnit.obj"
	-@erase "$(INTDIR)\ArcUnit.sbr"
	-@erase "$(INTDIR)\BaseUnit.obj"
	-@erase "$(INTDIR)\BaseUnit.sbr"
	-@erase "$(INTDIR)\BitmapUnit.obj"
	-@erase "$(INTDIR)\BitmapUnit.sbr"
	-@erase "$(INTDIR)\Combination.obj"
	-@erase "$(INTDIR)\Combination.sbr"
	-@erase "$(INTDIR)\DisjointUnit.obj"
	-@erase "$(INTDIR)\DisjointUnit.sbr"
	-@erase "$(INTDIR)\EllipseUnit.obj"
	-@erase "$(INTDIR)\EllipseUnit.sbr"
	-@erase "$(INTDIR)\EraseUnit.obj"
	-@erase "$(INTDIR)\EraseUnit.sbr"
	-@erase "$(INTDIR)\LineUnit.obj"
	-@erase "$(INTDIR)\LineUnit.sbr"
	-@erase "$(INTDIR)\PenBrushUnit.obj"
	-@erase "$(INTDIR)\PenBrushUnit.sbr"
	-@erase "$(INTDIR)\PolygonUnit.obj"
	-@erase "$(INTDIR)\PolygonUnit.sbr"
	-@erase "$(INTDIR)\PolylineUnit.obj"
	-@erase "$(INTDIR)\PolylineUnit.sbr"
	-@erase "$(INTDIR)\RectUnit.obj"
	-@erase "$(INTDIR)\RectUnit.sbr"
	-@erase "$(INTDIR)\RgnObjUnit.obj"
	-@erase "$(INTDIR)\RgnObjUnit.sbr"
	-@erase "$(INTDIR)\RoundRectUnit.obj"
	-@erase "$(INTDIR)\RoundRectUnit.sbr"
	-@erase "$(INTDIR)\SCT Draw Activex.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.pch"
	-@erase "$(INTDIR)\SCT Draw Activex.res"
	-@erase "$(INTDIR)\SCT Draw Activex.sbr"
	-@erase "$(INTDIR)\SCT Draw Activex.tlb"
	-@erase "$(INTDIR)\SCTDrawActivexCtl.obj"
	-@erase "$(INTDIR)\SCTDrawActivexCtl.sbr"
	-@erase "$(INTDIR)\SCTDrawActivexPpg.obj"
	-@erase "$(INTDIR)\SCTDrawActivexPpg.sbr"
	-@erase "$(INTDIR)\SectorUnit.obj"
	-@erase "$(INTDIR)\SectorUnit.sbr"
	-@erase "$(INTDIR)\SplitUnit.obj"
	-@erase "$(INTDIR)\SplitUnit.sbr"
	-@erase "$(INTDIR)\SprayBottleUnit.obj"
	-@erase "$(INTDIR)\SprayBottleUnit.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StylesDlg.obj"
	-@erase "$(INTDIR)\StylesDlg.sbr"
	-@erase "$(INTDIR)\TextUnit.obj"
	-@erase "$(INTDIR)\TextUnit.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SCT Draw Activex.bsc"
	-@erase "$(OUTDIR)\SCT Draw Activex.exp"
	-@erase "$(OUTDIR)\SCT Draw Activex.ilk"
	-@erase "$(OUTDIR)\SCT Draw Activex.lib"
	-@erase "$(OUTDIR)\SCT Draw Activex.ocx"
	-@erase "$(OUTDIR)\SCT Draw Activex.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SCT Draw Activex.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ArcUnit.sbr" \
	"$(INTDIR)\BaseUnit.sbr" \
	"$(INTDIR)\BitmapUnit.sbr" \
	"$(INTDIR)\Combination.sbr" \
	"$(INTDIR)\DisjointUnit.sbr" \
	"$(INTDIR)\EllipseUnit.sbr" \
	"$(INTDIR)\EraseUnit.sbr" \
	"$(INTDIR)\LineUnit.sbr" \
	"$(INTDIR)\PenBrushUnit.sbr" \
	"$(INTDIR)\PolygonUnit.sbr" \
	"$(INTDIR)\PolylineUnit.sbr" \
	"$(INTDIR)\RectUnit.sbr" \
	"$(INTDIR)\RgnObjUnit.sbr" \
	"$(INTDIR)\RoundRectUnit.sbr" \
	"$(INTDIR)\SCT Draw Activex.sbr" \
	"$(INTDIR)\SCTDrawActivexCtl.sbr" \
	"$(INTDIR)\SCTDrawActivexPpg.sbr" \
	"$(INTDIR)\SectorUnit.sbr" \
	"$(INTDIR)\SplitUnit.sbr" \
	"$(INTDIR)\SprayBottleUnit.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StylesDlg.sbr" \
	"$(INTDIR)\TextUnit.sbr"

"$(OUTDIR)\SCT Draw Activex.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=zlib.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SCT Draw Activex.pdb" /debug /machine:I386 /def:".\SCT Draw Activex.def" /out:"$(OUTDIR)\SCT Draw Activex.ocx" /implib:"$(OUTDIR)\SCT Draw Activex.lib" /pdbtype:sept 
DEF_FILE= \
	".\SCT Draw Activex.def"
LINK32_OBJS= \
	"$(INTDIR)\ArcUnit.obj" \
	"$(INTDIR)\BaseUnit.obj" \
	"$(INTDIR)\BitmapUnit.obj" \
	"$(INTDIR)\Combination.obj" \
	"$(INTDIR)\DisjointUnit.obj" \
	"$(INTDIR)\EllipseUnit.obj" \
	"$(INTDIR)\EraseUnit.obj" \
	"$(INTDIR)\LineUnit.obj" \
	"$(INTDIR)\PenBrushUnit.obj" \
	"$(INTDIR)\PolygonUnit.obj" \
	"$(INTDIR)\PolylineUnit.obj" \
	"$(INTDIR)\RectUnit.obj" \
	"$(INTDIR)\RgnObjUnit.obj" \
	"$(INTDIR)\RoundRectUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.obj" \
	"$(INTDIR)\SCTDrawActivexCtl.obj" \
	"$(INTDIR)\SCTDrawActivexPpg.obj" \
	"$(INTDIR)\SectorUnit.obj" \
	"$(INTDIR)\SplitUnit.obj" \
	"$(INTDIR)\SprayBottleUnit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StylesDlg.obj" \
	"$(INTDIR)\TextUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.res"

"$(OUTDIR)\SCT Draw Activex.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\SCT Draw Activex.ocx
InputPath=.\Debug\SCT Draw Activex.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"

OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\DebugU
# End Custom Macros

ALL : "$(OUTDIR)\SCT Draw Activex.ocx" "$(OUTDIR)\SCT Draw Activex.tlb" ".\DebugU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\ArcUnit.obj"
	-@erase "$(INTDIR)\BaseUnit.obj"
	-@erase "$(INTDIR)\BitmapUnit.obj"
	-@erase "$(INTDIR)\Combination.obj"
	-@erase "$(INTDIR)\DisjointUnit.obj"
	-@erase "$(INTDIR)\EllipseUnit.obj"
	-@erase "$(INTDIR)\EraseUnit.obj"
	-@erase "$(INTDIR)\LineUnit.obj"
	-@erase "$(INTDIR)\PenBrushUnit.obj"
	-@erase "$(INTDIR)\PolygonUnit.obj"
	-@erase "$(INTDIR)\PolylineUnit.obj"
	-@erase "$(INTDIR)\RectUnit.obj"
	-@erase "$(INTDIR)\RgnObjUnit.obj"
	-@erase "$(INTDIR)\RoundRectUnit.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.pch"
	-@erase "$(INTDIR)\SCT Draw Activex.res"
	-@erase "$(INTDIR)\SCT Draw Activex.tlb"
	-@erase "$(INTDIR)\SCTDrawActivexCtl.obj"
	-@erase "$(INTDIR)\SCTDrawActivexPpg.obj"
	-@erase "$(INTDIR)\SectorUnit.obj"
	-@erase "$(INTDIR)\SplitUnit.obj"
	-@erase "$(INTDIR)\SprayBottleUnit.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StylesDlg.obj"
	-@erase "$(INTDIR)\TextUnit.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SCT Draw Activex.exp"
	-@erase "$(OUTDIR)\SCT Draw Activex.ilk"
	-@erase "$(OUTDIR)\SCT Draw Activex.lib"
	-@erase "$(OUTDIR)\SCT Draw Activex.ocx"
	-@erase "$(OUTDIR)\SCT Draw Activex.pdb"
	-@erase ".\DebugU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\SCT Draw Activex.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SCT Draw Activex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SCT Draw Activex.pdb" /debug /machine:I386 /def:".\SCT Draw Activex.def" /out:"$(OUTDIR)\SCT Draw Activex.ocx" /implib:"$(OUTDIR)\SCT Draw Activex.lib" /pdbtype:sept 
DEF_FILE= \
	".\SCT Draw Activex.def"
LINK32_OBJS= \
	"$(INTDIR)\ArcUnit.obj" \
	"$(INTDIR)\BaseUnit.obj" \
	"$(INTDIR)\BitmapUnit.obj" \
	"$(INTDIR)\Combination.obj" \
	"$(INTDIR)\DisjointUnit.obj" \
	"$(INTDIR)\EllipseUnit.obj" \
	"$(INTDIR)\EraseUnit.obj" \
	"$(INTDIR)\LineUnit.obj" \
	"$(INTDIR)\PenBrushUnit.obj" \
	"$(INTDIR)\PolygonUnit.obj" \
	"$(INTDIR)\PolylineUnit.obj" \
	"$(INTDIR)\RectUnit.obj" \
	"$(INTDIR)\RgnObjUnit.obj" \
	"$(INTDIR)\RoundRectUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.obj" \
	"$(INTDIR)\SCTDrawActivexCtl.obj" \
	"$(INTDIR)\SCTDrawActivexPpg.obj" \
	"$(INTDIR)\SectorUnit.obj" \
	"$(INTDIR)\SplitUnit.obj" \
	"$(INTDIR)\SprayBottleUnit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StylesDlg.obj" \
	"$(INTDIR)\TextUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.res"

"$(OUTDIR)\SCT Draw Activex.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\DebugU
TargetPath=.\DebugU\SCT Draw Activex.ocx
InputPath=.\DebugU\SCT Draw Activex.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"

OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\ReleaseU
# End Custom Macros

ALL : "$(OUTDIR)\SCT Draw Activex.ocx" "$(OUTDIR)\SCT Draw Activex.tlb" ".\ReleaseU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\ArcUnit.obj"
	-@erase "$(INTDIR)\BaseUnit.obj"
	-@erase "$(INTDIR)\BitmapUnit.obj"
	-@erase "$(INTDIR)\Combination.obj"
	-@erase "$(INTDIR)\DisjointUnit.obj"
	-@erase "$(INTDIR)\EllipseUnit.obj"
	-@erase "$(INTDIR)\EraseUnit.obj"
	-@erase "$(INTDIR)\LineUnit.obj"
	-@erase "$(INTDIR)\PenBrushUnit.obj"
	-@erase "$(INTDIR)\PolygonUnit.obj"
	-@erase "$(INTDIR)\PolylineUnit.obj"
	-@erase "$(INTDIR)\RectUnit.obj"
	-@erase "$(INTDIR)\RgnObjUnit.obj"
	-@erase "$(INTDIR)\RoundRectUnit.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.obj"
	-@erase "$(INTDIR)\SCT Draw Activex.pch"
	-@erase "$(INTDIR)\SCT Draw Activex.res"
	-@erase "$(INTDIR)\SCT Draw Activex.tlb"
	-@erase "$(INTDIR)\SCTDrawActivexCtl.obj"
	-@erase "$(INTDIR)\SCTDrawActivexPpg.obj"
	-@erase "$(INTDIR)\SectorUnit.obj"
	-@erase "$(INTDIR)\SplitUnit.obj"
	-@erase "$(INTDIR)\SprayBottleUnit.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StylesDlg.obj"
	-@erase "$(INTDIR)\TextUnit.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SCT Draw Activex.exp"
	-@erase "$(OUTDIR)\SCT Draw Activex.lib"
	-@erase "$(OUTDIR)\SCT Draw Activex.ocx"
	-@erase ".\ReleaseU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\SCT Draw Activex.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SCT Draw Activex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SCT Draw Activex.pdb" /machine:I386 /def:".\SCT Draw Activex.def" /out:"$(OUTDIR)\SCT Draw Activex.ocx" /implib:"$(OUTDIR)\SCT Draw Activex.lib" 
DEF_FILE= \
	".\SCT Draw Activex.def"
LINK32_OBJS= \
	"$(INTDIR)\ArcUnit.obj" \
	"$(INTDIR)\BaseUnit.obj" \
	"$(INTDIR)\BitmapUnit.obj" \
	"$(INTDIR)\Combination.obj" \
	"$(INTDIR)\DisjointUnit.obj" \
	"$(INTDIR)\EllipseUnit.obj" \
	"$(INTDIR)\EraseUnit.obj" \
	"$(INTDIR)\LineUnit.obj" \
	"$(INTDIR)\PenBrushUnit.obj" \
	"$(INTDIR)\PolygonUnit.obj" \
	"$(INTDIR)\PolylineUnit.obj" \
	"$(INTDIR)\RectUnit.obj" \
	"$(INTDIR)\RgnObjUnit.obj" \
	"$(INTDIR)\RoundRectUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.obj" \
	"$(INTDIR)\SCTDrawActivexCtl.obj" \
	"$(INTDIR)\SCTDrawActivexPpg.obj" \
	"$(INTDIR)\SectorUnit.obj" \
	"$(INTDIR)\SplitUnit.obj" \
	"$(INTDIR)\SprayBottleUnit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StylesDlg.obj" \
	"$(INTDIR)\TextUnit.obj" \
	"$(INTDIR)\SCT Draw Activex.res"

"$(OUTDIR)\SCT Draw Activex.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseU
TargetPath=.\ReleaseU\SCT Draw Activex.ocx
InputPath=.\ReleaseU\SCT Draw Activex.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SCT Draw Activex.dep")
!INCLUDE "SCT Draw Activex.dep"
!ELSE 
!MESSAGE Warning: cannot find "SCT Draw Activex.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SCT Draw Activex - Win32 Release" || "$(CFG)" == "SCT Draw Activex - Win32 Debug" || "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug" || "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"
SOURCE=.\ArcUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\ArcUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\ArcUnit.obj"	"$(INTDIR)\ArcUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\ArcUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\ArcUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\BaseUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\BaseUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\BaseUnit.obj"	"$(INTDIR)\BaseUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\BaseUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\BaseUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\BitmapUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\BitmapUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\BitmapUnit.obj"	"$(INTDIR)\BitmapUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\BitmapUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\BitmapUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\Combination.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\Combination.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\Combination.obj"	"$(INTDIR)\Combination.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\Combination.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\Combination.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\DisjointUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\DisjointUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\DisjointUnit.obj"	"$(INTDIR)\DisjointUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\DisjointUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\DisjointUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\EllipseUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\EllipseUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\EllipseUnit.obj"	"$(INTDIR)\EllipseUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\EllipseUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\EllipseUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\EraseUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\EraseUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\EraseUnit.obj"	"$(INTDIR)\EraseUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\EraseUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\EraseUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\LineUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\LineUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\LineUnit.obj"	"$(INTDIR)\LineUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\LineUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\LineUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\PenBrushUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\PenBrushUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\PenBrushUnit.obj"	"$(INTDIR)\PenBrushUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\PenBrushUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\PenBrushUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\PolygonUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\PolygonUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\PolygonUnit.obj"	"$(INTDIR)\PolygonUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\PolygonUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\PolygonUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\PolylineUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\PolylineUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\PolylineUnit.obj"	"$(INTDIR)\PolylineUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\PolylineUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\PolylineUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\RectUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\RectUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\RectUnit.obj"	"$(INTDIR)\RectUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\RectUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\RectUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\RgnObjUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\RgnObjUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\RgnObjUnit.obj"	"$(INTDIR)\RgnObjUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\RgnObjUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\RgnObjUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\RoundRectUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\RoundRectUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\RoundRectUnit.obj"	"$(INTDIR)\RoundRectUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\RoundRectUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\RoundRectUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=".\SCT Draw Activex.cpp"

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SCT Draw Activex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SCT Draw Activex.obj"	"$(INTDIR)\SCT Draw Activex.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SCT Draw Activex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SCT Draw Activex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=".\SCT Draw Activex.odl"

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\SCT Draw Activex.tlb" /mktyplib203 /win32 

"$(OUTDIR)\SCT Draw Activex.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\SCT Draw Activex.tlb" /mktyplib203 /win32 

"$(OUTDIR)\SCT Draw Activex.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\SCT Draw Activex.tlb" /mktyplib203 /win32 

"$(OUTDIR)\SCT Draw Activex.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\SCT Draw Activex.tlb" /mktyplib203 /win32 

"$(OUTDIR)\SCT Draw Activex.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\SCT Draw Activex.rc"

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SCT Draw Activex.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.tlb"
	$(RSC) /l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SCT Draw Activex.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.tlb"
	$(RSC) /l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SCT Draw Activex.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /i "DebugU" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SCT Draw Activex.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x804 /fo"$(INTDIR)\SCT Draw Activex.res" /i "ReleaseU" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\SCTDrawActivexCtl.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SCTDrawActivexCtl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SCTDrawActivexCtl.obj"	"$(INTDIR)\SCTDrawActivexCtl.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SCTDrawActivexCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SCTDrawActivexCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\SCTDrawActivexPpg.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SCTDrawActivexPpg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SCTDrawActivexPpg.obj"	"$(INTDIR)\SCTDrawActivexPpg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SCTDrawActivexPpg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SCTDrawActivexPpg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\SectorUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SectorUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SectorUnit.obj"	"$(INTDIR)\SectorUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SectorUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SectorUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\SplitUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SplitUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SplitUnit.obj"	"$(INTDIR)\SplitUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SplitUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SplitUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\SprayBottleUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\SprayBottleUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\SprayBottleUnit.obj"	"$(INTDIR)\SprayBottleUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\SprayBottleUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\SprayBottleUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\SCT Draw Activex.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SCT Draw Activex.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SCT Draw Activex.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SCT Draw Activex.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\SCT Draw Activex.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SCT Draw Activex.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\SCT Draw Activex.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SCT Draw Activex.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StylesDlg.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\StylesDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\StylesDlg.obj"	"$(INTDIR)\StylesDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\StylesDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\StylesDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 

SOURCE=.\TextUnit.cpp

!IF  "$(CFG)" == "SCT Draw Activex - Win32 Release"


"$(INTDIR)\TextUnit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Debug"


"$(INTDIR)\TextUnit.obj"	"$(INTDIR)\TextUnit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Debug"


"$(INTDIR)\TextUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ELSEIF  "$(CFG)" == "SCT Draw Activex - Win32 Unicode Release"


"$(INTDIR)\TextUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SCT Draw Activex.pch"


!ENDIF 


!ENDIF 

