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

namespace frontend
{
    class LauncherClient;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class LinkInterpreter_ABC;
}

class Config;

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
             BattleCenterPage( QWidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter );
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
