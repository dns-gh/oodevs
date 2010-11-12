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

#include "LauncherClientPage.h"
#include "Profile.h"

namespace frontend
{
    class Config;
    class Exercise_ABC;
}

namespace kernel
{
    class Controllers;
}

class ProgressPage;
class ExerciseList;
class SessionList;

// =============================================================================
/** @class  ReplayPage
    @brief  ReplayPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ReplayPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPage( QWidgetStack* pages, Page_ABC& previous, const frontend::Config& config, kernel::Controllers& controllers, frontend::LauncherClient& launcher );
    virtual ~ReplayPage();
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnStart();
    void OnSelectExercise( const frontend::Exercise_ABC& exercise, const Profile& profile );
    void ClearSelection();
    void OnSelectSession( const QString& session );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplayPage( const ReplayPage& );            //!< Copy constructor
    ReplayPage& operator=( const ReplayPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    void StartExercise();
    void ConfigureSession( const QString& exercise, const QString& session );
    //@}

private:
    //! @name Member data
    //@{
    const frontend::Config& config_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    ExerciseList* exercises_;
    SessionList* sessions_;
    const frontend::Exercise_ABC* exercise_;
    Profile profile_;
    QString session_;
    QListBox* sessionList_;
    //@}
};

#endif // __ReplayPage_h_
