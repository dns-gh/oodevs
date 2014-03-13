// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef _LogisticStocksDialog_h
#define _LogisticStocksDialog_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"

class StocksAndNaturesEditor;
class StaticModel;

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class ModelUnLoaded;
}

// =============================================================================
/** @class  LogisticStocksDialog
    @brief  LogisticStocksDialog
*/
// Created: MMC 2013-10-24
// =============================================================================
class LogisticStocksDialog : public QDialog
                           , public tools::Observer_ABC
                           , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                           , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT
    
public:
    //! @name Constructors/Destructor
    //@{
             LogisticStocksDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~LogisticStocksDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Accept();
    void Reject();
    void ShowDialog();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    StocksAndNaturesEditor* stockAndNaturesEditor_;
    //@}
};

#endif // _LogisticStocksDialog_h
