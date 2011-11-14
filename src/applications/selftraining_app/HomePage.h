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

namespace tools
{
    class Loader_ABC;
}

class Config;
class MenuButton;
class OptionsPage;
class ScenarioEditPage;
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
             HomePage( QWidget* parent, Q3WidgetStack* pages, Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter );
    virtual ~HomePage();
    //@}

    //! @name Operation
    //@{
    void InstallPackage( const QString& package );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void OnOptions();
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    Config& config_;
    OptionsPage* optionsPage_;
    ScenarioEditPage* editPage_;
    MenuButton* adapt_;
    MenuButton* prepare_;
    MenuButton* play_;
    MenuButton* replay_;
    //@}
};

#endif // __HomePage_h_
