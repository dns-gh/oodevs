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

class ExerciseList;
class Profile;
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
             ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers );
    virtual ~ScenarioEditPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void Update();
    void CreateExercise();
    void EditExercise();
    void OnSelect( const QString& exercise, const Profile& profile );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioEditPage( const ScenarioEditPage& );            //!< Copy constructor
    ScenarioEditPage& operator=( const ScenarioEditPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Edit( const QString& exercise );
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
    QComboBox* editTerrainList_;
    QComboBox* editModelList_;
    QLineEdit* editName_;
    //@}
};

#endif // __ScenarioEditPage_h_
