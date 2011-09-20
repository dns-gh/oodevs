// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyPushFlowDialog_h_
#define __LogisticSupplyPushFlowDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/Resolver_ABC.h"
#include "clients_gui/ValuedComboBox.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class Dotation;
class StaticModel;
class SupplyStates;

// =============================================================================
/** @class  LogisticSupplyPushFlowDialog
    @brief  LogisticSupplyPushFlowDialog
*/
// $$$$ SBO 2006-07-03: look somehow similar to LogisticSupplyChangeQuotasDialog...
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyPushFlowDialog : public QDialog
                                   , public tools::Observer_ABC
                                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                   , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyPushFlowDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyPushFlowDialog();
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
    void OnRecipientValueChanged( int row, int /*col*/ );
    void OnRecipientSelectionChanged( int row, int /*col*/ );
    void OnResourcesValueChanged( int row, int /*col*/ );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticSupplyPushFlowDialog( const LogisticSupplyPushFlowDialog& );            //!< Copy constructor
    LogisticSupplyPushFlowDialog& operator=( const LogisticSupplyPushFlowDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddItem();
    void AddDotation( const SupplyStates& states );
    void InsertMenuEntry( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu );

    void AddRecipientItem();
    void AddResourceItem();
    void AddResourceItem( QString dotationName, int Available, int qtySupply );
    void AddCarrierItem();
    void AddWaypoint();

    void clearRecipientsTable();
    void clearRecipientsData();
    void clearResourcesTable();
    void clearResourcesData();
    void computeAvailableRecipients( QStringList& recipientsNames );
    void insertNewRecipientData( int index, const kernel::Automat_ABC* pRecipient );
    void eraseRecipientData( int index );
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, Dotation > T_Supplies;

    struct dotationQuantity 
    {
        QString dotationName_;
        int quantity_;
        dotationQuantity(): quantity_(0) {}
        dotationQuantity( const QString& dotationName, int quantity ): dotationName_( dotationName ), quantity_( quantity ) {}
    };

    typedef std::vector< dotationQuantity > T_SuppliesVector;

    typedef std::map< const kernel::Automat_ABC*, T_SuppliesVector > T_RecipientSupplies;    
    typedef std::vector< const kernel::Automat_ABC* > T_Recipients;
    typedef QMap< QString, const kernel::Automat_ABC* > T_RecipientsNames;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automats_;
    const kernel::Profile_ABC& profile_;

    gui::ValuedComboBox< const kernel::Automat_ABC* >* targetCombo_;
    Q3Table* table_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;

    Q3Table* recipientsTable_;
    Q3Table* resourcesTable_;
    Q3Table* carriersTable_;
    QListView* waypointList_;

    T_Recipients recipients_;
    T_RecipientSupplies recipientSupplies_;
    T_RecipientsNames recipientsNames_;

    const kernel::Automat_ABC* pRecipientSelected_;
    //@}
};

#endif // __LogisticSupplyPushFlowDialog_h_
