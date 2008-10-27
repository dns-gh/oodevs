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

#include "MenuPage.h"

namespace kernel
{
    class Controllers;
}

class Config;
class NetworkExerciseLister;

// =============================================================================
/** @class  BattleCenterPage
    @brief  BattleCenterPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class BattleCenterPage : public MenuPage
{

public:
    //! @name Constructors/Destructor
    //@{
             BattleCenterPage( QWidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister );
    virtual ~BattleCenterPage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BattleCenterPage( const BattleCenterPage& );            //!< Copy constructor
    BattleCenterPage& operator=( const BattleCenterPage& ); //!< Assignment operator
    //@}
};

#endif // __BattleCenterPage_h_
