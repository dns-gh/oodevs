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

namespace bfs = boost::filesystem;
using namespace license_gui;

#define ID_MAIN_EDIT 101
#define ID_BUTTON_EMAIL 102
#define ID_BUTTON_INSTALL 103
#define ID_BUTTON_CLOSE 104

#define MARGIN 10
#define BUTTON_HEIGHT 30

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
    HWND hWnd = CreateWindow( wc.lpszClassName, "Invalid License", WS_OVERLAPPEDWINDOW & ( ~ WS_THICKFRAME ),
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
            HWND hButtonEmail   = CreateWindow( "Button", "Send email",      buttonStyle, 0, 0, 0, 0, hWnd,  reinterpret_cast< HMENU >( ID_BUTTON_EMAIL   ), hInstance, NULL );
            HWND hButtonInstall = CreateWindow( "Button", "Install license", buttonStyle, 0, 0, 0, 0, hWnd,  reinterpret_cast< HMENU >( ID_BUTTON_INSTALL ), hInstance, NULL );
            HWND hButtonClose   = CreateWindow( "Button", "Close",           buttonStyle, 0, 0, 0, 0, hWnd,  reinterpret_cast< HMENU >( ID_BUTTON_CLOSE   ), hInstance, NULL );
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
            SetWindowPos( GetDlgItem( hWnd, ID_BUTTON_EMAIL ), NULL,
                          MARGIN,
                          height - MARGIN - BUTTON_HEIGHT,
                          buttonWidth,
                          BUTTON_HEIGHT, SWP_NOZORDER );
            SetWindowPos( GetDlgItem( hWnd, ID_BUTTON_INSTALL ), NULL,
                          2 * MARGIN + buttonWidth,
                          height - MARGIN - BUTTON_HEIGHT,
                          buttonWidth,
                          BUTTON_HEIGHT, SWP_NOZORDER );
            SetWindowPos( GetDlgItem( hWnd, ID_BUTTON_CLOSE ), NULL,
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
      << "&body=Name :%0ACompany :%0AAddress :%0APhone :%0AEmail :%0AComments :%0A%0APLEASE DON'T REMOVE THE FOLLOWING LINES%0A%0A"
      << "Feature : " << feature_ << "%0A"
      << "HostID : " << hostId_ << "%0A";
    ShellExecute ( hWnd, "open", s.str().c_str(), NULL, NULL, SW_SHOW );
}

// -----------------------------------------------------------------------------
// Name: LicenseDialog::SetEditText
// Created: JSR 2011-10-24
// -----------------------------------------------------------------------------
void LicenseDialog::SetEditText( HWND hEdit )
{
    std::stringstream s;
    s << "Please contact MASA Group by sending an email to licenses@masagroup.net with the following information :\r\n\r\n"
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
    ofn.lpstrFilter = "License Files (*.lic)\0*.lic\0\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = _T( "Please select license file..." );
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
