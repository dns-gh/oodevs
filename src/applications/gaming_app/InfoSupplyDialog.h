// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoSupplyDialog_h_
#define __InfoSupplyDialog_h_

#include "InfoDialog.h"
#include "gaming/SupplyStates.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class DisplayExtractor;
}

class SupplyStocksListView;
class SupplyQuotasWidget;
class SupplyStatusWidget;
class Publisher_ABC;

// =============================================================================
/** @class  InfoSupplyDialog
    @brief  InfoSupplyDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoSupplyDialog : public InfoDialog< SupplyStates >
                       , public tools::ElementObserver_ABC< SupplyStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoSupplyDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory
                             , gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Publisher_ABC& publisher );
    virtual ~InfoSupplyDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& entity ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const SupplyStates& dotations );
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget* tabs_;
    SupplyQuotasWidget* supplyQuotasWidget_;
    //@}
};

#endif // __InfoSupplyDialog_h_
