// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelfTrainingPage_h_
#define __SelfTrainingPage_h_

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
class ExerciseContainer;
class Config;
class ScenarioLauncherPage;

// =============================================================================
/** @class  SelfTrainingPage
    @brief  SelfTrainingPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class SelfTrainingPage : public MenuPage
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             SelfTrainingPage( Application& app, QStackedWidget* pages,
                               Page_ABC& previous, const Config& config,
                               const tools::Loader_ABC& fileLoader,
                               kernel::Controllers& controllers,
                               ExerciseContainer& exercises );
    virtual ~SelfTrainingPage();
    //@}

public:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnStart();
    //@}

private:
    //! @name Data Member
    //@{
    const Config& config_;
    ScenarioLauncherPage* launcher_;
    MenuButton* startButton_;
    MenuButton* joinButton_;
    //@}
};

#endif // __SelfTrainingPage_h_
