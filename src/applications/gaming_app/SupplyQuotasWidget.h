// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyQuotasWidget_h_
#define __SupplyQuotasWidget_h_

#include "LogisticStatusWidget.h"
#include "gaming/LogisticLinks.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  SupplyQuotasWidget
    @brief  SupplyQuotasWidget
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyQuotasWidget : public Q3VBox
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< LogisticLinks >
                         , public tools::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SupplyQuotasWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~SupplyQuotasWidget();
    //@}

public:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const LogisticLinks& links );
    virtual void NotifySelected( const kernel::Entity_ABC* pEntity );
    //@}

private slots:
    //! @name Helpers
    //@{
    virtual void UpdateQuotas();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void UpdateSuperiors( const LogisticLinks& links );
    virtual void UpdateQuotas( const LogisticLinks& links );
    //@}

    kernel::Controllers& controllers_;
    gui::ValuedComboBox< const kernel::Entity_ABC* >* superior_;
    QStandardItemModel* dataModel_;
    Q3HBox* hboxSuperior_;
    QTableView* tableView_;
    const LogisticLinks* pLinks_;
};

#endif // __SupplyQuotasWidget_h_
