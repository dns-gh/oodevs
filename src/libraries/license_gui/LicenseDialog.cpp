// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LicenseDialog.h"
#include "tools/win32/FlexLm.h"
#include "tools/FileWrapper.h"
#include "tools/Path.h"
#include <tchar.h>
#include <sstream>
#include <xeumeuleu/xml.hpp>

using namespace license_gui;

#define ID_MAIN_EDIT 1001
#define ID_BUTTON_EMAIL 1002
#define ID_BUTTON_INSTALL 1003
#define ID_BUTTON_CLOSE 1004

#define MARGIN 10
#define BUTTON_HEIGHT 30

#include <iostream>

namespace
{
    std::string ReadLang()
    {
        HKEY hkCle = NULL;
        char sValeur[ 200 ];
        DWORD dwTailleValeur1 = 200;
        LONG found = RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\MASA Group\\SWORD\\Common", 0, KEY_ALL_ACCESS, &hkCle );
        if( found == ERROR_SUCCESS )
        {
            RegQueryValueEx( hkCle, "Language", NULL, NULL, (LPBYTE)sValeur, &dwTailleValeur1 );
            RegCloseKey(hkCle);
        }
        if( !found && RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\MASA Group\\SWORD\\Common", 0, KEY_ALL_ACCESS, &hkCle ) == ERROR_SUCCESS )
        {
            RegQueryValueEx( hkCle, "Language", NULL, NULL, (LPBYTE)sValeur, &dwTailleValeur1 );
            RegCloseKey(hkCle);
            found = true;
        }
        if( !found )
            return "en";
        std::string result = sValeur;
        if( result.find( "fr" ) != std::string::npos )
            return "fr";
        if( result.find( "es" ) != std::string::npos )
            return "es";
        if( result.find( "ar" ) != std::string::npos )
            return "ar";
        return "en";
    }
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::CheckLicense
// Created: JSR 2011-10-27
// -----------------------------------------------------------------------------
void LicenseDialog::CheckLicense( const std::string& licenseFeature, bool silentMode /*= false*/, int* pMaxConnections /*= 0*/, std::string* pExpiration /*= 0*/  )
{
    for( ;; )
    {
        try
        {
            std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( licenseFeature, 1.0f, "license.dat;.", silentMode ? FlexLmLicense::eCheckModeSilent : FlexLmLicense::eCheckModeCustom ) );
            if( pMaxConnections )
            {
                try
                {
                    *pMaxConnections = license->GetAuthorisedUsers();
                }
                catch( const FlexLmLicense::LicenseError& )
                {
                    *pMaxConnections = 1;
                }
            }
            if( pExpiration )
                *pExpiration = license->GetExpirationDate();
            break;
        }
        catch( const FlexLmLicense::LicenseError& e )
        {
            if( silentMode )
                throw;
            if( !license_gui::LicenseDialog::Run( licenseFeature, e.hostid_ ) )
                throw;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::Run
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
bool LicenseDialog::Run( const std::string& feature, const std::string& hostId )
{
    bool installingLicense = false;
    TCHAR currentDirectory[ MAX_PATH ];
    GetCurrentDirectory( MAX_PATH, currentDirectory );
    LicenseDialog( feature, hostId, installingLicense );
    SetCurrentDirectory( currentDirectory );
    return installingLicense;
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog constructor
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
LicenseDialog::LicenseDialog( const std::string& feature, const std::string& hostId, bool& installingLicense )
    : installingLicense_( installingLicense )
    , feature_          ( feature )
    , hostId_           ( hostId )
{
    ReadTranslations();

    static TCHAR szWindowClass[] = _T( "win32app" );
    static TCHAR szTitle[] = _T( "License Dialog" );

    HINSTANCE hInstance = GetModuleHandle( NULL );

    WNDCLASS wc;
    wc.style = 0;
    wc.lpfnWndProc = &LicenseDialog::MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( 0, IDI_EXCLAMATION );
    wc.hCursor = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = reinterpret_cast< HBRUSH >( 1 + COLOR_BTNFACE );
    wc.lpszMenuName = 0;
    wc.lpszClassName = "LicenseWinClass";
    if( ! RegisterClass( &wc ) )
        return;
    HWND hWnd = CreateWindow( wc.lpszClassName, tr( "Invalid License" ).c_str(), WS_OVERLAPPEDWINDOW & ( ~ WS_THICKFRAME ),
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
        0, 0, hInstance, 0 );
    SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( this ) );

    RECT rc;
    int screenWidth = GetSystemMetrics( SM_CXSCREEN );
    int screenHeight = GetSystemMetrics( SM_CYSCREEN );
    GetWindowRect( hWnd, &rc );
    SetWindowPos( hWnd, 0, ( screenWidth - rc.right ) / 2, ( screenHeight - rc.bottom ) / 2 , 0, 0, SWP_NOSIZE );

    ShowWindow( hWnd, SW_SHOW );
    UpdateWindow(hWnd);

    MSG msg;
    BOOL bRet;
    while( ( bRet = GetMessage( &msg, NULL, 0, 0 ) ) != 0 )
    {
        if( bRet != -1 )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
    UnregisterClass( wc.lpszClassName, hInstance );
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog destructor
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
LicenseDialog::~LicenseDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::MainWndProc
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
LRESULT CALLBACK LicenseDialog::MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    LicenseDialog* dlg = reinterpret_cast< LicenseDialog* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    switch( uMsg )
    {
    case WM_CREATE:
        {
            HINSTANCE hInstance = GetModuleHandle( NULL );
            DWORD buttonStyle = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE;
            HWND hEdit = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 0, 0, 0, 0, hWnd, reinterpret_cast< HMENU >( ID_MAIN_EDIT ), hInstance, NULL );
            HWND hButtonEmail   = CreateWindow( "Button", "", buttonStyle, 0, 0, 0, 0, hWnd,  reinterpret_cast< HMENU >( ID_BUTTON_EMAIL   ), hInstance, NULL );
            HWND hButtonInstall = CreateWindow( "Button", "", buttonStyle, 0, 0, 0, 0, hWnd,  reinterpret_cast< HMENU >( ID_BUTTON_INSTALL ), hInstance, NULL );
            HWND hButtonClose   = CreateWindow( "Button", "", buttonStyle, 0, 0, 0, 0, hWnd,  reinterpret_cast< HMENU >( ID_BUTTON_CLOSE   ), hInstance, NULL );
            WPARAM hfDefault = reinterpret_cast<  WPARAM  >( GetStockObject( DEFAULT_GUI_FONT ) );
            SendMessage( hEdit,          WM_SETFONT, hfDefault, MAKELPARAM( FALSE, 0 ) );
            SendMessage( hButtonInstall, WM_SETFONT, hfDefault, MAKELPARAM( FALSE, 0 ) );
            SendMessage( hButtonEmail,   WM_SETFONT, hfDefault, MAKELPARAM( FALSE, 0 ) );
            SendMessage( hButtonClose,   WM_SETFONT, hfDefault, MAKELPARAM( FALSE, 0 ) );
        }
        break;
    case WM_SIZE:
        {
            RECT rcClient;
            GetClientRect( hWnd, &rcClient );
            LONG width = rcClient.right - rcClient.left;
            LONG height = rcClient.bottom - rcClient.top;
            LONG buttonWidth = ( width - 4 * MARGIN ) / 3;
            SetWindowPos( GetDlgItem( hWnd, ID_MAIN_EDIT ), NULL,
                          MARGIN,
                          MARGIN, width - 2 * MARGIN,
                          height - 3 * MARGIN - BUTTON_HEIGHT,
                          SWP_NOZORDER );
            HWND hButtonEmail   = GetDlgItem( hWnd, ID_BUTTON_EMAIL );
            HWND hButtonInstall = GetDlgItem( hWnd, ID_BUTTON_INSTALL );
            HWND hButtonClose   = GetDlgItem( hWnd, ID_BUTTON_CLOSE );
            const std::string label1 = "Send email";
            const std::string label2 = "Install license";
            const std::string label3 = "Close";
            SetWindowText( hButtonEmail,   ( dlg ? dlg->tr( label1 ) : label1 ).c_str() );
            SetWindowText( hButtonInstall, ( dlg ? dlg->tr( label2 ) : label2 ).c_str() );
            SetWindowText( hButtonClose,   ( dlg ? dlg->tr( label3 ) : label3 ).c_str() );
            SetWindowPos( hButtonEmail, NULL,
                          MARGIN,
                          height - MARGIN - BUTTON_HEIGHT,
                          buttonWidth,
                          BUTTON_HEIGHT, SWP_NOZORDER );
            SetWindowPos( hButtonInstall, NULL,
                          2 * MARGIN + buttonWidth,
                          height - MARGIN - BUTTON_HEIGHT,
                          buttonWidth,
                          BUTTON_HEIGHT, SWP_NOZORDER );
            SetWindowPos( hButtonClose, NULL,
                          3 * MARGIN + 2 * buttonWidth,
                          height - MARGIN - BUTTON_HEIGHT,
                          buttonWidth,
                          BUTTON_HEIGHT, SWP_NOZORDER );
        }
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case ID_BUTTON_EMAIL:
            if( dlg )
                dlg->SendMail( hWnd );
            DestroyWindow( hWnd );
            break;
        case ID_BUTTON_INSTALL:
            if( dlg )
                dlg->InstallLicense( hWnd );
            DestroyWindow( hWnd );
            break;
        case ID_BUTTON_CLOSE:
            DestroyWindow( hWnd );
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    case WM_SHOWWINDOW:
        if( dlg )
            dlg->SetEditText( GetDlgItem( hWnd, ID_MAIN_EDIT ) );
        break;
    default:
        return DefWindowProc( hWnd, uMsg, wParam, lParam );
        break;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::SendMail
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
void LicenseDialog::SendMail( HWND hWnd )
{
    std::stringstream s;
    s << "mailto:licenses@masagroup.net?subject= MASA Sword license request"
      << "&body="
      << tr( "Name" )     << " :%0A"
      << tr( "Company" )  << " :%0A"
      << tr( "Address" )  << " :%0A"
      << tr( "Phone" )    << " :%0A"
      << tr( "Email" )    << " :%0A"
      << tr( "Comments" ) << " :%0A%0A"
      << tr( "PLEASE DON'T REMOVE THE FOLLOWING LINES" ) << "%0A%0A"
      << "Feature : " << feature_ << "%0A"
      << "HostID : "  << hostId_  << "%0A";
    ShellExecute ( hWnd, "open", s.str().c_str(), NULL, NULL, SW_SHOW );
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::SetEditText
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
void LicenseDialog::SetEditText( HWND hEdit )
{
    std::stringstream s;
    s << tr( "Please contact MASA Group by sending an email to licenses@masagroup.net with the following information" ) << ":\r\n\r\n"
        << "Feature : " << feature_ << "\r\n"
        << "HostID : " << hostId_;
    SetWindowText( hEdit, s.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::InstallLicense
// Created: JSR 2011-10-25
// -----------------------------------------------------------------------------
void LicenseDialog::InstallLicense( HWND hWnd )
{
    OPENFILENAME ofn;
    TCHAR szFileName[ MAX_PATH ];
    strcpy_s( szFileName, "*.lic" );
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = hWnd;
    const std::string licenseFiles = tr( "License Files" ) + " (*.lic)\0*.lic\0\0";
    ofn.lpstrFilter = licenseFiles.c_str();
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    static const std::string title = tr( "Please select license file..." );
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "lic";
    if( GetOpenFileName( &ofn ) )
    {
        TCHAR szAppPath[ MAX_PATH ];
        if( GetModuleFileName( NULL, szAppPath, MAX_PATH ) )
        {
            try
            {
                tools::Path sourceFile( szFileName );
                tools::Path destFile( szAppPath );
                destFile = destFile.Parent();
                destFile /= sourceFile.FileName();
                if( destFile.Exists() )
                    destFile.Remove();
                sourceFile.Copy( destFile );
                installingLicense_ = true;
            }
            catch( ... )
            {
                // NOTHING
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::ReadTranslations
// Created: JSR 2011-10-26
// -----------------------------------------------------------------------------
void LicenseDialog::ReadTranslations()
{
    try
    {
        TCHAR szAppPath[ MAX_PATH ];
        if( GetModuleFileName( NULL, szAppPath, MAX_PATH ) )
        {
            tools::Path filePath( szAppPath );
            filePath = filePath.Parent();
            const std::string language = ReadLang();
            filePath /= tools::Path( "resources/locales" ) / "license_gui_" + language.c_str() + ".ts";
            if( filePath.Exists() )
            {
                tools::Xifstream xis( filePath );
                xis >> xml::start( "TS" )
                      >> xml::start( "context" )
                        >> xml::list( "message", *this, &LicenseDialog::ReadTranslation );
            }
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::ReadTranslation
// Created: JSR 2011-10-26
// -----------------------------------------------------------------------------
void LicenseDialog::ReadTranslation( xml::xistream& xis )
{
    const std::string message = xis.content< std::string >( "source", "" );
    const std::string translation = xis.content< std::string >( "translation", "" );
    if( !message.empty() && !translation.empty() )
        translations_[ message ] = translation;
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::tr
// Created: JSR 2011-10-26
// -----------------------------------------------------------------------------
const std::string& LicenseDialog::tr( const std::string& source ) const
{
    std::map< std::string, std::string >::const_iterator it = translations_.find( source );
    if( it != translations_.end() )
        return it->second;
    return source;
}
