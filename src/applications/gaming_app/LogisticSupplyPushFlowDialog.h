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

#include "LogisticSupplyFlowDialog_ABC.h"

class LogisticSupplyExclusiveListWidget;

// =============================================================================
/** @class  LogisticSupplyPushFlowDialog
    @brief  LogisticSupplyPushFlowDialog
*/
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyPushFlowDialog : public LogisticSupplyFlowDialog_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyPushFlowDialog( QWidget* parent,
                                           kernel::Controllers& controllers,
                                           actions::ActionsModel& actionsModel,
                                           const ::StaticModel& staticModel,
                                           const kernel::Time_ABC& simulation,
                                           gui::ParametersLayer& layer,
                                           const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                           const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyPushFlowDialog();
    //@}

    //! @name Operations
    //@{
    void PushFlow( const kernel::Entity_ABC& entity );
    void Supply( const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void Validate();
    virtual void Reject();
    void AddRecipient( const QString& recipientName );
    void RemoveRecipient( const QString& recipientName );
    void OnRecipientSelectionChanged( const QString& currentRecipient, const QString& previousRecipient );
    //@}

private:
    //! @name Helpers
    //@{
    void GetSuppliesFromTable( const kernel::Automat_ABC& recipient );
    void SetSuppliesToTable( const kernel::Automat_ABC& recipient );

    void Clear();
    void Show();
    void ClearRecipientsData();
    void ComputeRecipients();
    void ComputeAvailableRecipients( QStringList& displayRecipientsNames );
    void EraseRecipientData( const QString& recipient );
    void ComputeAvailableCarriers( QMap< QString, int >& availableCarriers );
    //@}

    //! @name Types
    //@{
     typedef std::vector< const kernel::Automat_ABC* > T_Recipients;
     typedef QMap< QString, const kernel::Automat_ABC* > T_RecipientsNames;
     typedef std::map< const kernel::Automat_ABC*, T_QuantitiesMap > T_RecipientSupplies;
    //@}

    //! @name Helpers
    //@{
    virtual void ComputeRoute( T_Route& route );
    virtual void UpdateRouteDrawpoints();
    //@}

private:
    //! @name Member data
    //@{
    bool isPushFlow_;
    LogisticSupplyExclusiveListWidget* recipientsList_;
    T_Recipients recipients_;
    T_RecipientSupplies recipientSupplies_;
    T_RecipientsNames recipientsNames_;
    const kernel::Automat_ABC* pRecipientSelected_;
    //@}
};

#endif // __LogisticSupplyPushFlowDialog_h_
