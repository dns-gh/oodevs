// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ControllerToolbar_h_
#define __ControllerToolbar_h_

#include "astec_kernel/Observer_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/OptionsObserver_ABC.h"

class Team_ABC;
class Controllers;

// =============================================================================
/** @class  ControllerToolbar
    @brief  Toolbar for admin tools (changing team, launching the sim...) 
*/
// Created: APE 2004-09-21
// =============================================================================
class ControllerToolbar : public QToolBar
                        , public Observer_ABC
                        , public ElementObserver_ABC< Team_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ControllerToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~ControllerToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTeamChanged( int nValue );
    void LaunchScipio();
    void LaunchScipioDbg();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team_ABC& );
    virtual void NotifyDeleted( const Team_ABC& );
    //@}

private:
    //! @name Slots
    //@{
    ControllerToolbar( const ControllerToolbar& );
    ControllerToolbar& operator=( const ControllerToolbar& );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    std::vector< const Team_ABC* > teams_;
    QComboBox* pTeamCombo_;
    //@}
};


#endif // __ControllerToolbar_h_
