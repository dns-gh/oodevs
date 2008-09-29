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

class Session ; 

// =============================================================================
/** @class  TrayMenu
    @brief  TrayMenu
*/
// Created: RDS 2008-08-21
// =============================================================================
class TrayMenu : public QPopupMenu
{
    
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit TrayMenu( QWidget* mainWindow );
    virtual ~TrayMenu();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrayMenu( const TrayMenu& );            //!< Copy constructor
    TrayMenu& operator=( const TrayMenu& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update(); 
    //@}

private slots:
    //! @name Member data
    //@{
    void OnAboutToShow(); 
    void OnQuit(); 
    //@}

private:

    const Session*    currentSession_ ;  

};

#endif // __TrayMenu_h_
