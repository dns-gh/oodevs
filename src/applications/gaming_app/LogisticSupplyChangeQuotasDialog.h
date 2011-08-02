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
    class Profile_ABC;
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

// =============================================================================
/** @class  LogisticSupplyChangeQuotasDialog
    @brief  LogisticSupplyChangeQuotasDialog
*/
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyChangeQuotasDialog : public QDialog
                                       , public tools::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                       , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyChangeQuotasDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const Model& model, const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyChangeQuotasDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
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
    //! @name Copy/Assignment
    //@{
    LogisticSupplyChangeQuotasDialog( const LogisticSupplyChangeQuotasDialog& );            //!< Copy constructor
    LogisticSupplyChangeQuotasDialog& operator=( const LogisticSupplyChangeQuotasDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddItem();
    void AddDotation( const SupplyStates& states );
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, Dotation > T_Supplies;
    //@}

private:
    struct SelectedHolder
    {
        SelectedHolder(kernel::Controllers& controllers);
        const kernel::Entity_ABC* Selected () const;
        operator const kernel::Entity_ABC* () const;
        void Set( const kernel::Automat_ABC& agent );
        void Set( const kernel::Formation_ABC& agent );
        actions::Parameter_ABC* GetParameter(const kernel::OrderParameter& parameter, kernel::Controller& controller);
        void Reset();
        const std::string GetMagicActionType() const;

    private:
        kernel::SafePointer< kernel::Automat_ABC > selectedAutomat_;
        kernel::SafePointer< kernel::Formation_ABC > selectedFormation_;
    };
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const Model& model_;
    const kernel::Profile_ABC& profile_;

    gui::ValuedComboBox< const kernel::Entity_ABC* >* targetCombo_;
    Q3Table* table_;
    SelectedHolder selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;
    //@}
};

#endif // __LogisticSupplyChangeQuotasDialog_h_
