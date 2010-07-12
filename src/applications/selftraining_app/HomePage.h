// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HomePage_h_
#define __HomePage_h_

#include "MenuPage.h"

namespace kernel
{
    class Controllers;
}

class Config;
class MenuButton;
class NetworkExerciseLister;
class QuitPage;

// =============================================================================
/** @class  HomePage
    @brief  HomePage
*/
// Created: SBO 2008-02-21
// =============================================================================
class HomePage : public MenuPage
{
public:
    //! @name Constructors/Destructor
    //@{
             HomePage( QWidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister );
    virtual ~HomePage();
    //@}

public:
    //! @name Helpers
    //@{
    virtual void show();
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    MenuButton* adapt_;
    MenuButton* import_;
    MenuButton* prepare_;
    MenuButton* play_;
    MenuButton* replay_;
    //@}
};

#endif // __HomePage_h_
