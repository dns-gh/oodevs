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

#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Controllers;
}

// =============================================================================
/** @class  ControllerToolbar
    @brief  Toolbar for admin tools (changing team, launching the sim...) 
*/
// Created: APE 2004-09-21
// =============================================================================
class ControllerToolbar : public QToolBar
                        , public kernel::Observer_ABC
                        , public kernel::ElementObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ControllerToolbar( QMainWindow* pParent, kernel::Controllers& controllers );
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
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );
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
    kernel::Controllers& controllers_;
    std::vector< const kernel::Team_ABC* > teams_;
    QComboBox* pTeamCombo_;
    //@}
};


#endif // __ControllerToolbar_h_
