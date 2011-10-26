// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LicenseDialog.h"
#include <tchar.h>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "xeumeuleu/xml.hpp"
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;
using namespace license_gui;

#define ID_MAIN_EDIT 101
#define ID_BUTTON_EMAIL 102
#define ID_BUTTON_INSTALL 103
#define ID_BUTTON_CLOSE 104

#define MARGIN 10
#define BUTTON_HEIGHT 30

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", "SWORD" );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::Run
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
void LicenseDialog::Run( const std::string& feature, const std::string& hostId )
{
    LicenseDialog( feature, hostId );
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog constructor
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
LicenseDialog::LicenseDialog( const std::string& feature, const std::string& hostId )
    : feature_( feature )
    , hostId_ ( hostId )
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
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
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
            break;
        case ID_BUTTON_INSTALL:
            if( dlg )
                dlg->InstallLicense( hWnd );
            break;
        case ID_BUTTON_CLOSE:
            PostQuitMessage( 0 );
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
    strcpy_s( szFileName, ( feature_ + ".lic" ).c_str() );
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = hWnd;
    const std::string licenseFiles = tr( "License Files" ) + " (*.lic)\0*.lic\0\0";
    ofn.lpstrFilter = licenseFiles.c_str();
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = tr( "Please select license file..." ).c_str();
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "lic";
    if( GetOpenFileName( &ofn ) )
    {
        TCHAR szAppPath[ MAX_PATH ];
        if( GetModuleFileName( NULL, szAppPath, MAX_PATH ) )
        {
            try
            {
                bfs::path sourceFile( szFileName );
                bfs::path destFile( szAppPath );
                destFile.remove_filename();
                destFile /= sourceFile.leaf();
                if( bfs::exists( destFile) )
                    bfs::remove( destFile );
                bfs::copy_file( sourceFile, destFile );
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
            bfs::path filePath( szAppPath );
            filePath.remove_filename();
            const std::string language = ReadLang().ascii();
            filePath /= bfs::path( "resources/locales" ) / bfs::path( "license_gui_" + language + ".ts" );
            if( bfs::exists( filePath) )
            {
                xml::xifstream xis( filePath.native_file_string() );
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
