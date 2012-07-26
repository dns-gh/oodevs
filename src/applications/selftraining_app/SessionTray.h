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

#include <boost/noncopyable.hpp>

class QSystemTrayIcon;

// =============================================================================
/** @class  SessionTray
    @brief  SessionTray
*/
// Created: RDS 2008-09-26
// =============================================================================
class SessionTray : private boost::noncopyable
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
    //! @name Member data
    //@{
    QSystemTrayIcon& trayIcon_;
    //@}
};

#endif // __SessionTray_h_
