// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayPage_h_
#define __ReplayPage_h_

#include "ContentPage.h"
#include "frontend/Profile.h"

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
class SessionList;

// =============================================================================
/** @class  ReplayPage
    @brief  ReplayPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ReplayPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPage( Application& app, QStackedWidget* pages, Page_ABC& previous,
                         const Config& config,
                         const tools::Loader_ABC& fileLoader,
                         kernel::Controllers& controllers,
                         ExerciseContainer& exercises,
                         const frontend::DebugConfig* debug );
    virtual ~ReplayPage();
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnStart();
    void OnSelectExercise( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    void OnSelectSession( const tools::Path& session );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void Update();
    void StartExercise();
    tools::Path ConfigureSession( const tools::Path& exercise, const tools::Path& session );
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    const tools::Loader_ABC& fileLoader_;
    kernel::Controllers& controllers_;
    ExerciseContainer& exerciseContainer_;
    const frontend::DebugConfig* debug_;
    ProgressPage* progressPage_;
    ExerciseList* exercises_;
    SessionList* sessions_;
    const frontend::Exercise_ABC* exercise_;
    frontend::Profile profile_;
    tools::Path session_;
    //@}
};

#endif // __ReplayPage_h_
