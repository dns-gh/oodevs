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

// =============================================================================
/** @class  BorrowingsListView
    @brief  BorrowingsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class BorrowingsListView : public ResourcesListView_ABC< Borrowings >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             BorrowingsListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~BorrowingsListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Borrowings& a );
    //@}
};

#endif // __BorrowingsListView_h_
