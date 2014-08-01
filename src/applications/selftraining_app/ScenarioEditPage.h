// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ScenarioEditPage_h_
#define __ScenarioEditPage_h_

#include "ContentPage.h"

namespace frontend
{
    class Config;
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
class CreateExerciceWidget;
class ExerciseContainer;
class ExerciseList;
class ProgressPage;

// =============================================================================
/** @class  ScenarioEditPage
    @brief  ScenarioEditPage
*/
// Created: RDS 2008-09-09
// =============================================================================
class ScenarioEditPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioEditPage( Application& app, QWidget* parent, QStackedWidget* pages,
                               Page_ABC& previous, const frontend::Config& config,
                               const tools::Loader_ABC& fileLoader,
                               kernel::Controllers& controllers,
                               ExerciseContainer& exerciseContainer,
                               const frontend::DebugConfig& debug );
    virtual ~ScenarioEditPage();
    //@}

public:
    //! @name Helpers
    //@{
    void Edit( const tools::Path& exercise );
    bool ExerciceExists( const tools::Path& string );
    void LaunchScenarioImport( const tools::Path& inputScenario, const tools::Path& outputScenario );
    void LaunchPreparation( const tools::Path& outputScenario );
    void UpdateEditButton();
    //@}

private:
    //! @name Type
    //@{
    enum E_Tabs { eTabs_Edit = 0, eTabs_Create = 1 };
    //@}

    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnEdit();
    void Update();
    void OnSelect( const frontend::Exercise_ABC& exercise );
    void OnExercisePropertiesChanged();
    void ClearSelection();
    void EditNameChanged( const QString& string );
    void ComboChanged( int index );
    void UpdateEditButton( QWidget* );
    void ToggleChanged( bool on );
    //@}

private:
    //! @name Member data
    //@{
    const frontend::Config& config_;
    const tools::Loader_ABC& fileLoader_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    ExerciseList* exercises_;
    const frontend::Exercise_ABC* exercise_;
    QTabWidget* mainTabs_;
    CreateExerciceWidget* createExerciceWidget_;
    ExerciseContainer& exerciseContainer_;
    const frontend::DebugConfig& debug_;
    //@}
};

#endif // __ScenarioEditPage_h_
