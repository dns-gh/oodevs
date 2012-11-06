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

// =============================================================================
/** @class  LendingsListView
    @brief  LendingsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class LendingsListView : public ResourcesListView_ABC< Lendings >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LendingsListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~LendingsListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Lendings& a );
    //@}
};

#endif // __LendingsListView_h_
