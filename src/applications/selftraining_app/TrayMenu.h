// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __TrayMenu_h_
#define __TrayMenu_h_

class Session;

// =============================================================================
/** @class  TrayMenu
    @brief  TrayMenu
*/
// Created: RDS 2008-08-21
// =============================================================================
class TrayMenu : public QMenu
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit TrayMenu( QWidget* mainWindow );
    virtual ~TrayMenu();
    //@}

private slots:
    //! @name Member data
    //@{
    void OnQuit();
    //@}

private:
    //! @name Member data
    //@{
    QWidget* main_;
    //@}
};

#endif // __TrayMenu_h_
