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

#include "astec_gaming/ContextMenuObserver_ABC.h"
#include "astec_gaming/SafePointer.h"
#include "ValuedComboBox.h"
#include <qdialog.h>

class Controllers;
class Model;
class Agent_ABC;
class Dotation;
class Publisher_ABC;

// =============================================================================
/** @class  LogisticSupplyChangeQuotasDialog
    @brief  LogisticSupplyChangeQuotasDialog
*/
// Created: SBO 2006-07-03
// =============================================================================
class LogisticSupplyChangeQuotasDialog : public QDialog
                                       , public Observer_ABC
                                       , public ContextMenuObserver_ABC< Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyChangeQuotasDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Model& model );
    virtual ~LogisticSupplyChangeQuotasDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu );
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
    Publisher_ABC& publisher_;
    const Model& model_;
    ValuedComboBox< const Agent_ABC* >* targetCombo_;
    QTable* table_;
    SafePointer< Agent_ABC > selected_;
    QStringList dotationTypes_;
    T_Supplies supplies_;
    //@}
};

#endif // __LogisticSupplyChangeQuotasDialog_h_
