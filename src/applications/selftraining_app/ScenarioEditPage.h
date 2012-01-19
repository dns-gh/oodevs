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

#include "LauncherClientPage.h"

namespace frontend
{
    class Config;
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

class CreateExerciceWidget;
class ExerciseList;
class ExportWidget;
class ImportWidget;
class ProgressPage;

// =============================================================================
/** @class  ScenarioEditPage
    @brief  ScenarioEditPage
*/
// Created: RDS 2008-09-09
// =============================================================================
class ScenarioEditPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioEditPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, const frontend::Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher );
    virtual ~ScenarioEditPage();
    //@}

public:
    //! @name Helpers
    //@{
    void Edit( const QString& exercise );
    bool ExerciceExists( const QString& string );
    void LaunchScenarioImport( const QString& inputScenario, const QString& outputScenario );
    void ShowPackageInstallation( const QString& package );
    void LaunchPreparation( const QString& outputScenario );
    void UpdateEditButton();
    //@}

private:
    //! @name Type
    //@{
    enum E_Tabs { eTabs_Edit = 0, eTabs_Create = 1, eTabs_Import = 2, eTabs_Export = 3 };
    //@}

    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnEdit();
    virtual void OnDelete();
    void Update();
    void OnSelect( const frontend::Exercise_ABC& exercise );
    void ClearSelection();
    void EditNameChanged( const QString& string );
    void ComboChanged( int index );
    void UpdateEditButton( QWidget* );
    void ToggleChanged( bool on );
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    const frontend::Config& config_;
    const tools::Loader_ABC& fileLoader_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    ExerciseList* exercises_;
    const frontend::Exercise_ABC* exercise_;
    QTabWidget* mainTabs_;
    CreateExerciceWidget* createExerciceWidget_;
    ImportWidget* importWidget_;
    ExportWidget* exportWidget_;
    //@}
};

#endif // __ScenarioEditPage_h_
