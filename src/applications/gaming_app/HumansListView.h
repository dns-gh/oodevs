// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HumansListView_h_
#define __HumansListView_h_

#include "gaming/Troops.h"
#include "gaming/Humans.h"
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
/** @class  HumansListView
    @brief  HumansListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class HumansListView : public ResourcesListView_ABC< HumansListView, Troops >
{

public:
    //! @name Constructors/Destructor
    //@{
             HumansListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~HumansListView();
    //@}

    //! @name Operations
    //@{
    void Display( const Humans& humans, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Troops& a );
    //@}
};

#endif // __HumansListView_h_
