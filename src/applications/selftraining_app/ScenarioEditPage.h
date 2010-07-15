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
#include "DirectoryExerciseLister.h"

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

class CreateExerciceWidget;
class ExerciseList;
class ExportWidget;
class ImportWidget;
class Profile;
class ProgressPage;
class QTabWidget;

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
             ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers );
    virtual ~ScenarioEditPage();
    //@}

public:
    //! @name Helpers
    //@{
    void Edit( const QString& exercise );
    bool ExerciceExists( const QString& string );
    void LaunchScenarioImport( const QString& inputScenario, const QString& outputScenario );
    void LaunchPreparation( const QString& outputScenario );
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnEdit();
    void Update();
    void OnSelect( const QString& exercise, const Profile& profile );
    void EditNameChanged( const QString& string );
    void ComboChanged( int index );
    void UpdateEditButton( QWidget* );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioEditPage( const ScenarioEditPage& );            //!< Copy constructor
    ScenarioEditPage& operator=( const ScenarioEditPage& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    kernel::Controllers& controllers_;
    DirectoryExerciseLister lister_;
    ProgressPage* progressPage_;
    ExerciseList* exercises_;
    QString exercise_;
    QTabWidget* mainTabs_;
    CreateExerciceWidget* createExerciceWidget_;
    ImportWidget* importWidget_;
    ExportWidget* exportWidget_;
    //@}
};

#endif // __ScenarioEditPage_h_
