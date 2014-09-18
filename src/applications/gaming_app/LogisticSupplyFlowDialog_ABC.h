// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyFlowDialog_ABC_h_
#define __LogisticSupplyFlowDialog_ABC_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class EquipmentType;
    class ModelUnLoaded;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class EntitySymbols;
}

namespace sword
{
    class UnitMagicActionAck;
}

class Dotation;
class LogisticRouteWidget;
class LogisticSupplyAvailabilityTableWidget;
class LogisticSupplyCarriersTableWidget;
class StaticModel;

// =============================================================================
/** @class  LogisticSupplyFlowDialog_ABC
    @brief  LogisticSupplyFlowDialog_ABC
*/
// Created: MMC 2012-10-18
// =============================================================================
class LogisticSupplyFlowDialog_ABC : public QDialog
                                   , public tools::Observer_ABC
                                   , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT;

protected:
    //! @name Constructors/Destructor
    //@{
    LogisticSupplyFlowDialog_ABC( QWidget* parent,
                                  kernel::Controllers& controllers,
                                  actions::ActionsModel& actionsModel,
                                  const ::StaticModel& staticModel,
                                  const kernel::Time_ABC& simulation,
                                  const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                  const kernel::Profile_ABC& profile,
                                  const gui::EntitySymbols& symbols );
    virtual ~LogisticSupplyFlowDialog_ABC();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Validate() = 0;
    virtual void Reject() = 0;
    virtual void closeEvent( QCloseEvent* /*pEvent*/ );
    virtual void OnCarriersUseCheckStateChanged();
    virtual void ClearCarriersData();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< QString, Dotation > T_Supplies;
    typedef std::map< QString, Dotation > T_AvailableDotations;
    typedef QMap< QString, int > T_QuantitiesMap;
    typedef std::map< QString , const kernel::EquipmentType* > T_CarriersName;
    //@}

    //! @name Helpers
    //@{
    static QString GetErrorText( const sword::UnitMagicActionAck& ack );
    void EnableButtons( bool enabled );

    virtual void AddAvailable( const Dotation& dotation );
    virtual void AddCarryingEquipment( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& model );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const ::StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automats_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QStringList dotationTypes_;
    T_AvailableDotations availableSupplies_;
    T_QuantitiesMap carriersTypes_;
    T_CarriersName carriersTypeNames_;
    QTabWidget* tabs_;
    LogisticSupplyAvailabilityTableWidget* resourcesTable_;
    LogisticSupplyCarriersTableWidget* carriersTable_;
    QCheckBox* carriersUseCheck_;
    QWidget* resourcesTab_;
    LogisticRouteWidget* route_;
    QPushButton* ok_;
    QPushButton* cancel_;
    //@}
};

#endif // __LogisticSupplyFlowDialog_ABC_h_
