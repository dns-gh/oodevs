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

namespace tools
{
    class Loader_ABC;
}

class Application;
class Config;
class ExerciseContainer;
class MenuButton;
class AuthoringPage;
class OptionsPage;
class ScenarioEditPage;
class QuitPage;
class SelfTrainingPage;
class ReplayPage;

// =============================================================================
/** @class  HomePage
    @brief  HomePage
*/
// Created: SBO 2008-02-21
// =============================================================================
class HomePage : public MenuPage
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             HomePage( Application& app, QWidget* parent, QStackedWidget* pages,
                       Config& config, const tools::Loader_ABC& fileLoader,
                       kernel::Controllers& controllers,
                       ExerciseContainer& exercises );
    virtual ~HomePage();
    //@}

    //! @name Operation
    //@{
    void InstallPackage( const tools::Path& package );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void OnOptions();
    void Update();
    //@}

private slots:
    void OnPrepare();
    void OnReplay();

private:
    //! @name Member data
    //@{
    Config& config_;
    AuthoringPage* adaptPage_;
    OptionsPage* optionsPage_;
    ScenarioEditPage* editPage_;
    SelfTrainingPage* playPage_;
    ReplayPage* replayPage_;
    MenuButton* adapt_;
    MenuButton* prepare_;
    MenuButton* play_;
    MenuButton* replay_;
    //@}
};

#endif // __HomePage_h_
