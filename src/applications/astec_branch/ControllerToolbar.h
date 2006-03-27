// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ControllerToolbar.h $
// $Author: Age $
// $Modtime: 14/04/05 12:53 $
// $Revision: 2 $
// $Workfile: ControllerToolbar.h $
//
// *****************************************************************************

#ifndef __ControllerToolbar_h_
#define __ControllerToolbar_h_

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "OptionsObserver_ABC.h"

class Team;
class Controllers;

// =============================================================================
/** @class  ControllerToolbar
    @brief  Toolbar for admin tools (changing team, launching the sim...) 
*/
// Created: APE 2004-09-21
// =============================================================================
class ControllerToolbar : public QToolBar
                        , public Observer_ABC
                        , public ElementObserver_ABC< Team >
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
    virtual void NotifyCreated( const Team& );
    virtual void NotifyDeleted( const Team& );
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
    std::vector< const Team* > teams_;
    QComboBox* pTeamCombo_;
    //@}
};


#endif // __ControllerToolbar_h_
