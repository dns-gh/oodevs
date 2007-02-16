// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LendingsListView_h_
#define __LendingsListView_h_

#include "gaming/Loan.h"
#include "gaming/Lendings.h"
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
/** @class  LendingsListView
    @brief  LendingsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class LendingsListView : public ResourcesListView_ABC< LendingsListView, Lendings >
{

public:
    //! @name Constructors/Destructor
    //@{
             LendingsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~LendingsListView();
    //@}

    //! @name Operations
    //@{
    void Display( const Loan& loan, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Lendings& a );
    //@}
};

#endif // __LendingsListView_h_
