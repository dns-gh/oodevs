// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AuthoringPage_h_
#define __AuthoringPage_h_

#include "MenuPage.h"

namespace kernel
{
    class Controllers;
}

class Config;
class MenuButton;
class ProgressPage;

// =============================================================================
/** @class  AuthoringPage
    @brief  AuthoringPage
*/
// Created: JSR 2010-06-04
// =============================================================================
class AuthoringPage : public MenuPage
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AuthoringPage( Q3WidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers );
    virtual ~AuthoringPage();
    //@}

public:
    //! @name Helpers
    //@{
    virtual void show();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAuthoring();
    void OnTerrainWorkshop();
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    MenuButton* authoring_;
    MenuButton* terrainGen_;
    MenuButton* terrainWorkshop_;
    //@}
};

#endif // __AuthoringPage_h_
