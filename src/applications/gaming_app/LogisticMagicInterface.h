// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogisticMagicInterface_h
#define __LogisticMagicInterface_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/Observer_ABC.h>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Automat_ABC;
    class ContextMenu;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
}

// =============================================================================
/** @class  LogisticMagicInterface
    @brief  LogisticMagicInterface
*/
// Created: ABR 2014-01-21
// =============================================================================
class LogisticMagicInterface : public QObject
                             , public tools::Observer_ABC
                             , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    LogisticMagicInterface( QObject* parent,
                            kernel::Controllers& controllers,
                            actions::ActionsModel& actionsModel );
    virtual ~LogisticMagicInterface();
    //@}

private slots:
    //! @name Slots
    //@{
    void SwitchMaintenance();
    //@}

private:
    //! @name OBservers implementations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

    //! @name Helpers
    //@{
    void AddSetMaintenanceManual( kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __LogisticMagicInterface_h
