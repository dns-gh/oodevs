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

#include "ContextMenuObserver_ABC.h"
#include "SafePointer.h"
#include "ValuedComboBox.h"
#include <qdialog.h>

class Controllers;
class Model;
class Agent;
class Dotation;

// =============================================================================
/** @class  LogisticSupplyPushFlowDialog
    @brief  LogisticSupplyPushFlowDialog
*/
// $$$$ SBO 2006-07-03: look somehow similar to LogisticSupplyChangeQuotasDialog...
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyPushFlowDialog : public QDialog
                                   , public Observer_ABC
                                   , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyPushFlowDialog( QWidget* parent, Controllers& controllers, const Model& model );
    virtual ~LogisticSupplyPushFlowDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Agent& agent, QPopupMenu& menu );
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
    LogisticSupplyPushFlowDialog( const LogisticSupplyPushFlowDialog& );            //!< Copy constructor
    LogisticSupplyPushFlowDialog& operator=( const LogisticSupplyPushFlowDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddItem();
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, const Dotation* > T_Supplies;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Model& model_;
    ValuedComboBox< const Agent* >* targetCombo_;
    QTable* table_;
    SafePointer< Agent > selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;
    //@}
};

#endif // __LogisticSupplyPushFlowDialog_h_
