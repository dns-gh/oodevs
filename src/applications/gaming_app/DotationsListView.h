// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationsListView_h_
#define __DotationsListView_h_

#include "gaming/Dotation.h"
#include "gaming/Dotations.h"
#include "ResourcesListView_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

namespace gui
{
    class ValuedListItem;
}

// =============================================================================
/** @class  DotationsListView
    @brief  DotationsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class DotationsListView : public ResourcesListView_ABC< DotationsListView, Dotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~DotationsListView();
    //@}

    //! @name Operations
    //@{
    void Display( const Dotation& dotation, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Dotations& a );
    //@}
};

#endif // __DotationsListView_h_
