!include nsDialogs.nsh

Var InstallService

Function ServiceInstallationDialog.Prepare

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE
    !insertmacro MUI_HEADER_TEXT_PAGE $(SERVICE_TEXT_INSTALLMODE_TITLE) $(SERVICE_TEXT_INSTALLMODE_SUBTITLE)
    
    nsDialogs::Create 1018
    Pop $0

    ${NSD_CreateGroupBox} 0u 70u 100% 30u $(SERVICE_INSTALLPAGE_TEXT_TOP)
    Pop $0

    ${NSD_CreateCheckbox} 10u 85u 80% 10u $(SERVICE_INSTALLPAGE_TEXT_INSTALL)
    Pop $InstallService

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW
    nsDialogs::Show

FunctionEnd

Function ServiceInstallationDialog.Leave
    SendMessage $InstallService ${BM_GETCHECK} 0 0 $0

     ${if} $0 = ${BST_CHECKED}
        ExecWait '"$INSTDIR\applications\launcher_app.exe" "--install"'
        ExecWait '"$INSTDIR\applications\launcher_app.exe" "--start"'
     ${endif}

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE
FunctionEnd

Section
SectionEnd