// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyChangeQuotasDialog_h_
#define __LogisticSupplyChangeQuotasDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/ValuedComboBox.h"
#include "gaming/Dotation.h"

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
    class Controllers;
    class Controller;
    class OrderParameter;
    class Entity_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
    class Parameter_ABC;
}

class Dotation;
class Model;
class StaticModel;
class SupplyStates;
class LogisticSupplyValuesTableWidget;

// =============================================================================
/** @class  LogisticSupplyChangeQuotasDialog
    @brief  LogisticSupplyChangeQuotasDialog
*/
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyChangeQuotasDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyChangeQuotasDialog( QWidget* parent,
                                               kernel::Controllers& controllers,
                                               actions::ActionsModel& actionsModel,
                                               const StaticModel& staticModel,
                                               const kernel::Time_ABC& simulation,
                                               const Model& model );
    virtual ~LogisticSupplyChangeQuotasDialog();
    //@}

    //! @name Operations
    //@{
    void Show( const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    void OnSelectionChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void AddDotation( const Dotation& dotation );
    //@}

    //! @name Types
    //@{
    typedef QMap< QString, Dotation > T_Supplies;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const ::StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const Model& model_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    gui::ValuedComboBox< const kernel::Entity_ABC* >* targetCombo_;
    LogisticSupplyValuesTableWidget* table_;
    QStringList dotationTypes_;
    T_Supplies supplies_;
    //@}
};

#endif // __LogisticSupplyChangeQuotasDialog_h_
