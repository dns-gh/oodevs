// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BattleCenterPage_h_
#define __BattleCenterPage_h_

#include "ContentPage.h"

// =============================================================================
/** @class  BattleCenterPage
    @brief  BattleCenterPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class BattleCenterPage : public ContentPage
{

public:
    //! @name Constructors/Destructor
    //@{
             BattleCenterPage( QWidgetStack* pages, Page_ABC& previous );
    virtual ~BattleCenterPage();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BattleCenterPage( const BattleCenterPage& );            //!< Copy constructor
    BattleCenterPage& operator=( const BattleCenterPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __BattleCenterPage_h_
