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

#include "ContextMenuObserver_ABC.h"
#include "SafePointer.h"
#include "ValuedComboBox.h"
#include <qdialog.h>

class Controllers;
class Model;
class Agent;
class Dotation;

// =============================================================================
/** @class  LogisticSupplyChangeQuotasDialog
    @brief  LogisticSupplyChangeQuotasDialog
*/
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyChangeQuotasDialog : public QDialog
                                       , public Observer_ABC
                                       , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyChangeQuotasDialog( QWidget* parent, Controllers& controllers, const Model& model );
    virtual ~LogisticSupplyChangeQuotasDialog();
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
    LogisticSupplyChangeQuotasDialog( const LogisticSupplyChangeQuotasDialog& );            //!< Copy constructor
    LogisticSupplyChangeQuotasDialog& operator=( const LogisticSupplyChangeQuotasDialog& ); //!< Assignement operator
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

#endif // __LogisticSupplyChangeQuotasDialog_h_
