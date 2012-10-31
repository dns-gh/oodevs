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

#include "gaming/TroopsCompatibilityVersion.h"
#include "ResourcesListView_ABC.h"

class Humans;

// =============================================================================
/** @class  HumansListView
    @brief  HumansListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class HumansListView : public ResourcesListView_ABC< TroopsCompatibilityVersion >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             HumansListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~HumansListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const TroopsCompatibilityVersion& a );
    //@}
};

#endif // __HumansListView_h_
