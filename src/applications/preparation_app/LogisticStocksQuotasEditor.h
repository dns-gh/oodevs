// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _LogisticStocksQuotasEditor_h
#define _LogisticStocksQuotasEditor_h

#include "LogisticEditor.h" // todo for typedef
#include "MaxStockNaturesTable.h" // todo for typedef
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
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class ModelUnLoaded;
}

// =============================================================================
/** @class  LogisticStocksQuotasEditor
@brief  LogisticStocksQuotasEditor
*/
// Created: MMC 2013-10-24
// =============================================================================
class LogisticStocksQuotasEditor : public QDialog
                                 , public tools::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                                 , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT
    
public:
    //! @name Constructors/Destructor
    //@{
             LogisticStocksQuotasEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~LogisticStocksQuotasEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Types
    //@{
    class CustomTabWidget : public QTabWidget
    {
    public:
        CustomTabWidget( QWidget* parent ) : QTabWidget( parent ) {}
        virtual ~CustomTabWidget() {}

        void HideTabBar() { if( tabBar() ) tabBar()->hide(); }
        void ShowTabBar() { if( tabBar() ) tabBar()->show(); }
    };
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
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
    CustomTabWidget* tabs_;
    StocksAndNaturesEditor* stockAndNaturesEditor_;
    QuotasEditor* quotasEditor_;
    int stocksTabIndex_;
    int quotasTabIndex_;
    gui::RichPushButton* automaticEditButton_;
    //@}
};

#endif // _LogisticStocksQuotasEditor_h
