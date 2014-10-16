// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LockMapViewController_h_
#define __LockMapViewController_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "gaming/Simulation.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Population_ABC;
}

namespace simulation
{
    class sEndTick;
}

namespace gui
{
    class GLView_ABC;
}

// =============================================================================
/** @class  LockMapViewController
    @brief  LockMapViewController
*/
// Created: MMC 2013-09-05
// =============================================================================
class LockMapViewController : public QObject
                            , public tools::Observer_ABC
                            , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                            , public tools::ElementObserver_ABC< Simulation::sEndTick >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LockMapViewController( kernel::Controllers& controllers, gui::GLView_ABC& view );
    virtual ~LockMapViewController();
    //@}

    //! @name Operations
    //@{
    void Clear();
    //@}

private slots:
    //! @name Slots
    //@{
    void LockViewOnEntity();
    void UnlockViewOnEntity();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation::sEndTick& endTick );
    virtual void UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::GLView_ABC& view_;
    kernel::SafePointer< kernel::Entity_ABC > locked_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    geometry::Point2f center_;
    //@}
};

#endif // __LockMapViewController_h_
