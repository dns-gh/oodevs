// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LauncherClientPage_h_
#define __LauncherClientPage_h_

#include "ContentPage.h"
#include "frontend/ConnectionHandler_ABC.h"

namespace frontend
{
    class LauncherClient;
}

// =============================================================================
/** @class  LauncherClientPage
    @brief  LauncherClientPage
*/
// Created: SBO 2010-10-25
// =============================================================================
class LauncherClientPage : public ContentPage
                         , public frontend::ConnectionHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LauncherClientPage( Q3WidgetStack* pages, const QString& title, Page_ABC& previous, unsigned short buttonFlags, frontend::LauncherClient& launcher );
    virtual ~LauncherClientPage();
    //@}

    //! @name Operations
    //@{
    void Connect( const QString& host, unsigned short port );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherClientPage( const LauncherClientPage& );            //!< Copy constructor
    LauncherClientPage& operator=( const LauncherClientPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void OnConnectionSucceeded();
    virtual void OnConnectionFailed( const std::string& reason );
    virtual void OnConnectionLost( const std::string& reason );
    virtual void OnError( const std::string& message );
    //@}

private:
    //! @name Member data
    //@{
    frontend::LauncherClient& launcher_;
    //@}
};

#endif // __LauncherClientPage_h_
