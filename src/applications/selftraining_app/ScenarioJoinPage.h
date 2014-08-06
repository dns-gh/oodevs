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

#include "ContentPage.h"

namespace frontend
{
    struct DebugConfig;
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
class ExerciseContainer;
class ExerciseList;
class ProgressPage;
class QSpinBox;

// =============================================================================
/** @class  ScenarioJoinPage
    @brief  ScenarioJoinPage
*/
// Created: SBO 2008-10-14
// =============================================================================
class ScenarioJoinPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioJoinPage( Application& app, QStackedWidget* pages,
                               Page_ABC& previous, kernel::Controllers& controllers,
                               const Config& config, const tools::Loader_ABC& fileLoader,
                               ExerciseContainer& exercises,
                               const frontend::DebugConfig& debug );
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
    ExerciseContainer& exerciseContainer_;
    const Config& config_;
    const frontend::DebugConfig& debug_;
    const tools::Loader_ABC& fileLoader_;
    QTabWidget* tabs_;
    ProgressPage* progressPage_;
    QLineEdit* host_;
    QSpinBox* port_;
    QSpinBox* timeline_;
    ExerciseList* exercises_;
    const frontend::Exercise_ABC* exercise_;

    QLabel* hostLabel_;
    QLabel* portLabel_;
    QLabel* timelineLabel_;
    //@}
};

#endif // __ScenarioJoinPage_h_
