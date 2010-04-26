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
    class Controllers;
    class Automat_ABC;
    class Entity_ABC;
    class Profile_ABC;
}

namespace actions
{
    class ActionsModel;
}

class Dotation;
class Model;
class StaticModel;
class Simulation;

// =============================================================================
/** @class  LogisticSupplyChangeQuotasDialog
    @brief  LogisticSupplyChangeQuotasDialog
*/
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyChangeQuotasDialog : public QDialog
                                       , public tools::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyChangeQuotasDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, const Model& model, const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyChangeQuotasDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void OnSelectionChanged();
    void OnValueChanged( int row, int col );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticSupplyChangeQuotasDialog( const LogisticSupplyChangeQuotasDialog& );            //!< Copy constructor
    LogisticSupplyChangeQuotasDialog& operator=( const LogisticSupplyChangeQuotasDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddItem();
    void AddDotation( const kernel::Entity_ABC& entity );
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, Dotation > T_Supplies;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const Simulation& simulation_;
    const Model& model_;
    const kernel::Profile_ABC& profile_;

    gui::ValuedComboBox< const kernel::Automat_ABC* >* targetCombo_;
    QTable* table_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;
    //@}
};

#endif // __LogisticSupplyChangeQuotasDialog_h_
