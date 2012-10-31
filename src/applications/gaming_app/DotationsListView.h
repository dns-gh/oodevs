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

// =============================================================================
/** @class  DotationsListView
    @brief  DotationsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class DotationsListView : public ResourcesListView_ABC< kernel::Dotations_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DotationsListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~DotationsListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Dotations_ABC& a );
    //@}
};

#endif // __DotationsListView_h_
