// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BorrowingsListView_h_
#define __BorrowingsListView_h_

#include "gaming/Loan.h"
#include "gaming/Borrowings.h"
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
/** @class  BorrowingsListView
    @brief  BorrowingsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class BorrowingsListView : public ResourcesListView_ABC< BorrowingsListView, Borrowings >
{

public:
    //! @name Constructors/Destructor
    //@{
             BorrowingsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~BorrowingsListView();
    //@}

    //! @name Operations
    //@{
    void Display( const Loan& loan, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Borrowings& a );
    //@}
};

#endif // __BorrowingsListView_h_
