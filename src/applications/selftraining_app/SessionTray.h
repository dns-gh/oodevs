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

class TrayIcon;
class TrayMenu;

// =============================================================================
/** @class  SessionTray
    @brief  SessionTray
*/
// Created: RDS 2008-09-26
// =============================================================================
class SessionTray
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SessionTray( QWidget *parent );
    virtual ~SessionTray();
    //@}

public:
    //! @name Operations
    //@{
    void OnLanguageChanged();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SessionTray( const SessionTray& );            //!< Copy constructorS
    SessionTray& operator=( const SessionTray& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TrayMenu& trayMenu_;
    TrayIcon& trayIcon_;
    //@}
};

#endif // __SessionTray_h_
