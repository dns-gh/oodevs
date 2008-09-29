// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __SessionTray_h_
#define __SessionTray_h_

#include "clients_kernel/ElementObserver_ABC.h" 
#include "clients_kernel/Observer_ABC.h" 

namespace kernel
{
    class Controllers; 
}

class SessionStatus ; 
class TrayIcon; 
class TrayMenu; 

// =============================================================================
/** @class  SessionTray
    @brief  SessionTray
*/
// Created: RDS 2008-09-26
// =============================================================================
class SessionTray : public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< SessionStatus > 
{

public:
    //! @name Constructors/Destructor
    //@{
             SessionTray( QWidget *parent, kernel::Controllers& controllers );
    virtual ~SessionTray();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const SessionStatus& status );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SessionTray( const SessionTray& );            //!< Copy constructorS
    SessionTray& operator=( const SessionTray& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_ ; 
    TrayMenu&            trayMenu_ ; 
    TrayIcon&            trayIcon_ ; 
    //@}
};

#endif // __SessionTray_h_
