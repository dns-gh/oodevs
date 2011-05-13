;AddItem macro
!macro AddItem Path
    FileWrite $UninstLog "${Path}$\r$\n"
!macroend

;File macro
!macro File FilePath FileName
    IfFileExists "$OUTDIR\${FileName}" +2
    FileWrite $UninstLog "$OUTDIR\${FileName}$\r$\n"
    File "${FilePath}${FileName}"
!macroend

;CreateShortcut macro
!macro CreateShortcut FilePath FilePointer Pamameters Icon IconIndex
    FileWrite $UninstLog "${FilePath}$\r$\n"
    CreateShortcut "${FilePath}" "${FilePointer}" "${Pamameters}" "${Icon}" "${IconIndex}"
!macroend

;Copy files macro
!macro CopyFiles SourcePath DestPath
    IfFileExists "${DestPath}" +2
    FileWrite $UninstLog "${DestPath}$\r$\n"
    CopyFiles "${SourcePath}" "${DestPath}"
!macroend

;Rename macro
!macro Rename SourcePath DestPath
    IfFileExists "${DestPath}" +2
    FileWrite $UninstLog "${DestPath}$\r$\n"
    Rename "${SourcePath}" "${DestPath}"
!macroend

;CreateDirectory macro
!macro CreateDirectory Path
    CreateDirectory "${Path}"
    FileWrite $UninstLog "${Path}$\r$\n"
!macroend

;SetOutPath macro
!macro SetOutPath Path
    SetOutPath "${Path}"
    FileWrite $UninstLog "${Path}$\r$\n"
!macroend
 
;WriteUninstaller macro
!macro WriteUninstaller Path
    WriteUninstaller "${Path}"
    FileWrite $UninstLog "${Path}$\r$\n"
!macroend

;WriteRegStr macro
!macro WriteRegStr RegRoot UnInstallPath Key Value
    FileWrite $UninstLog "${RegRoot} ${UnInstallPath}$\r$\n"
    WriteRegStr "${RegRoot}" "${UnInstallPath}" "${Key}" "${Value}"
!macroend

;WriteRegDWORD macro
!macro WriteRegDWORD RegRoot UnInstallPath Key Value
    FileWrite $UninstLog "${RegRoot} ${UnInstallPath}$\r$\n"
    WriteRegStr "${RegRoot}" "${UnInstallPath}" "${Key}" "${Value}"
!macroend
  
;--------------------------------
; Configure UnInstall log to only remove what is installed
;-------------------------------- 
;Set the name of the uninstall log
!ifndef UninstLog
    !define UninstLog "uninstall.log"
!endif
Var UninstLog
 
;Uninstall log file missing.
LangString UninstLogMissing ${LANG_ENGLISH} "${UninstLog} not found!$\r$\nUninstallation cannot proceed!"

!define AddItem "!insertmacro AddItem"
!define File "!insertmacro File"
!define CreateShortcut "!insertmacro CreateShortcut"
!define CopyFiles "!insertmacro CopyFiles"
!define Rename "!insertmacro Rename"
!define CreateDirectory "!insertmacro CreateDirectory"
!define SetOutPath "!insertmacro SetOutPath"
!define WriteUninstaller "!insertmacro WriteUninstaller"
!define WriteRegStr "!insertmacro WriteRegStr"
!define WriteRegDWORD "!insertmacro WriteRegDWORD" 

Section -openlogfile
    CreateDirectory "$INSTDIR"
    IfFileExists "$INSTDIR\${UninstLog}" +3
        FileOpen $UninstLog "$INSTDIR\${UninstLog}" w
    Goto +4
    SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
    FileOpen $UninstLog "$INSTDIR\${UninstLog}" a
    FileSeek $UninstLog 0 END
SectionEnd

!macro UninstallFromLog
    ;Can't uninstall if uninstall log is missing!
    IfFileExists "$INSTDIR\${UninstLog}" +3
        MessageBox MB_OK|MB_ICONSTOP "$(UninstLogMissing)"
        Abort

    Push $R0
    Push $R1
    Push $R2
    SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
    FileOpen $UninstLog "$INSTDIR\${UninstLog}" r
    StrCpy $R1 -1

    GetLineCount:
        ClearErrors
        FileRead $UninstLog $R0
        IntOp $R1 $R1 + 1
        StrCpy $R0 $R0 -2
        Push $R0
        IfErrors 0 GetLineCount

    Pop $R0

    LoopRead:
        StrCmp $R1 0 LoopDone
        Pop $R0

        IfFileExists "$R0\*.*" 0 +3
            RMDir $R0  #is dir
        Goto LoopNext
        IfFileExists $R0 0 +3
            Delete $R0 #is file
        Goto LoopNext
;        StrCmp $R0 "${REG_ROOT} ${REG_APP_PATH}" 0 +3
;            DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}" #is Reg Element
;        Goto LoopNext
;        StrCmp $R0 "${REG_ROOT} ${UNINSTALL_PATH}" 0 +2
;            DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}" #is Reg Element
    LoopNext:
        IntOp $R1 $R1 - 1
        Goto LoopRead
    LoopDone:
    FileClose $UninstLog
    Delete "$INSTDIR\${UninstLog}"
    Pop $R2
    Pop $R1
    Pop $R0
    ;Remove registry keys
    ;DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
    ;DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
!macroend
