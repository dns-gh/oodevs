// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TutorialPage_h_
#define __TutorialPage_h_

#include "ContentPage.h"
#include "DirectoryExerciseLister.h"

namespace gui 
{
    class LinkInterpreter_ABC;
}

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
/** @class  TutorialPage
    @brief  TutorialPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class TutorialPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TutorialPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, gui::LinkInterpreter_ABC& interpreter );
    virtual ~TutorialPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnSelectExercise( const QString& exercise, const Profile& profile );
    void OnStart();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TutorialPage( const TutorialPage& );            //!< Copy constructor
    TutorialPage& operator=( const TutorialPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    void CreateSession( const QString& exercise, const QString& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    kernel::Controllers&        controllers_;
    gui::LinkInterpreter_ABC&   interpreter_;
    ProgressPage*               progressPage_;
    ExerciseList*               exercises_;
    QString                     exercise_;
    DirectoryExerciseLister     lister_;
    //@}
};

#endif // __TutorialPage_h_
