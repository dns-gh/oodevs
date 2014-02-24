// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyPullFlowDialog_h_
#define __LogisticSupplyPullFlowDialog_h_

#include "LogisticSupplyFlowDialog_ABC.h"

namespace kernel
{
    class Profile_ABC;
}

// =============================================================================
/** @class  LogisticSupplyPullFlowDialog
    @brief  LogisticSupplyPullFlowDialog
*/
// Created : AHC 2010-10-14
// =============================================================================
class LogisticSupplyPullFlowDialog : public LogisticSupplyFlowDialog_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyPullFlowDialog( QWidget* parent,
                                           kernel::Controllers& controllers,
                                           actions::ActionsModel& actionsModel,
                                           const ::StaticModel& staticModel,
                                           const kernel::Time_ABC& simulation,
                                           gui::ParametersLayer& layer,
                                           const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                           const tools::Resolver_ABC< kernel::Formation_ABC >& formations,
                                           const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyPullFlowDialog();
    //@}

    //! @name Operations
    //@{
    void Show( const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void Validate();
    virtual void Reject();
    void OnSupplierValueChanged();
    void OnSupplierSelectionChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void AddResourceItem();
    void ClearSuppliersTable();
    void ClearSuppliersData();
    void ComputeAvailableSuppliers();
    void SetSuppliesToTable();
    void ComputeAvailableCarriers( QMap< QString, int >& availableCarriers );
    void Clear();
    //@}

    //! @name Types
    //@{
     typedef QMap< QString, const kernel::Entity_ABC* > T_SuppliersNames;
    //@}

    //! @name Helpers
    //@{
    virtual void ComputeRoute( T_Route& route );
    virtual void UpdateRouteDrawpoints();
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Formation_ABC >& formations_;
    gui::ValuedComboBox< const kernel::Entity_ABC* >* supplierCombo_;
    const kernel::Entity_ABC* supplier_;
    T_SuppliersNames suppliersNames_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __LogisticSupplyPullFlowDialog_h_
