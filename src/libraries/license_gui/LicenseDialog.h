// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LicenseDialog_h_
#define __LicenseDialog_h_

#include <map>
#include <string>
#include <windows.h>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace license_gui
{

// =============================================================================
/** @class  LicenseDialog
    @brief  LicenseDialog
*/
// Created: JSR 2011-10-24
// =============================================================================
class LicenseDialog : private boost::noncopyable
{
public:
    //! @name Operations
    //@{
    static void Run( const std::string& feature, const std::string& hostId );
    //@}
    
private:
    //! @name Constructors/Destructor
    //@{
             LicenseDialog( const std::string& feature, const std::string& hostId );
    virtual ~LicenseDialog();
    //@}

private:
    //! @name Helpers
    //@{
    static LRESULT CALLBACK MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    void SetEditText( HWND hEdit );
    void SendMail( HWND hWnd );
    void InstallLicense( HWND hWnd );
    void ReadTranslations();
    void ReadTranslation( xml::xistream& xis );
    const std::string& tr( const std::string& source ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string feature_;
    const std::string hostId_;
    std::map< std::string, std::string > translations_;
    //@}
};

}

#endif // __LicenseDialog_h_
