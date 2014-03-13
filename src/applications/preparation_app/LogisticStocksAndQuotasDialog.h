// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _LogisticStocksAndQuotasDialog_h
#define _LogisticStocksAndQuotasDialog_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"

class LogisticQuotaEditor;
class LogisticStockEditor;
class QuotasEditor;
class StocksAndNaturesEditor;
class StaticModel;

namespace gui
{
    class RichPushButton;
}

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class ModelUnLoaded;
}

// =============================================================================
/** @class  LogisticStocksAndQuotasDialog
@brief  LogisticStocksAndQuotasDialog
*/
// Created: MMC 2013-10-24
// =============================================================================
class LogisticStocksAndQuotasDialog : public QDialog
                                    , public tools::Observer_ABC
                                    , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                    , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                                    , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT
    
public:
    //! @name Constructors/Destructor
    //@{
             LogisticStocksAndQuotasDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~LogisticStocksAndQuotasDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Accept();
    void Reject();
    void ShowAutomaticDialog();
    void ShowDialog();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    LogisticStockEditor* automaticStocksEditor_;
    LogisticQuotaEditor* automaticQuotaEditor_;
    QTabWidget* tabs_;
    StocksAndNaturesEditor* stockAndNaturesEditor_;
    QuotasEditor* quotasEditor_;
    //@}
};

#endif // _LogisticStocksAndQuotasDialog_h
