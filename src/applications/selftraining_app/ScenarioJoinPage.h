// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ScenarioJoinPage_h_
#define __ScenarioJoinPage_h_

#include "LauncherClientPage.h"

namespace frontend
{
    class Exercise_ABC;
}

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
class ExerciseList;
class ProgressPage;
class QSpinBox;

// =============================================================================
/** @class  ScenarioJoinPage
    @brief  ScenarioJoinPage
*/
// Created: SBO 2008-10-14
// =============================================================================
class ScenarioJoinPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioJoinPage( Application& app, Q3WidgetStack* pages,
                               Page_ABC& previous, kernel::Controllers& controllers,
                               const Config& config, const tools::Loader_ABC& fileLoader,
                               frontend::LauncherClient& launcher );
    virtual ~ScenarioJoinPage();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void Update();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnJoin();
    void SelectExercise( const frontend::Exercise_ABC& exercise );
    void ClearSelection();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Config& config_;
    const tools::Loader_ABC& fileLoader_;
    ProgressPage* progressPage_;
    QLineEdit* host_;
    QSpinBox* port_;
    ExerciseList* exercises_;
    const frontend::Exercise_ABC* exercise_;

    QLabel* hostLabel_;
    QLabel* portLabel_;
    //@}
};

#endif // __ScenarioJoinPage_h_
