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
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class ParametersLayer;
}

class LogisticSupplyChangeQuotasDialog;
class LogisticSupplyPullFlowDialog;
class LogisticSupplyPushFlowDialog;
class Model;
class StaticModel;

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
    LogisticMagicInterface( QWidget* parent,
                            kernel::Controllers& controllers,
                            Model& model,
                            const StaticModel& staticModel,
                            const kernel::Time_ABC& simulation,
                            const kernel::Profile_ABC& profile,
                            gui::ParametersLayer& layer );
    virtual ~LogisticMagicInterface();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeQuotas();
    void OnPullFlow();
    void OnPushFlow();
    void OnResupply();
    void OnSwitchMaintenanceMode();
    void OnSwitchSupplyMode();
    //@}

private:
    //! @name OBservers implementations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

    //! @name Helpers
    //@{
    void AddMenuEntries( kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    const kernel::Profile_ABC& profile_;
    LogisticSupplyChangeQuotasDialog* changeQuotasDialog_;
    LogisticSupplyPushFlowDialog* pushFlowDialog_;
    LogisticSupplyPullFlowDialog* pullFlowDialog_;
    //@}
};

#endif // __LogisticMagicInterface_h
