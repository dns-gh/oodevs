// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __RemoteControlPage_h_
#define __RemoteControlPage_h_

#include "LauncherClientPage.h"

namespace frontend
{
    class Exercise_ABC;
    class ExerciseFilter_ABC;
}

namespace kernel
{
    class Controllers;
}

class Config;
class ExerciseList;
class QSpinBox;

// =============================================================================
/** @class  RemoteControlPage
    @brief  RemoteControlPage
*/
// Created: SBO 2010-10-21
// =============================================================================
class RemoteControlPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             RemoteControlPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, frontend::LauncherClient& launcher );
    virtual ~RemoteControlPage();
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectExercise( const frontend::Exercise_ABC& exercise );
    void ClearSelection();
    void SelectRunningExercise( const frontend::Exercise_ABC& exercise );
    void ClearRunningSelection();

    void UpdateExerciseList();
    void OnStartSession();
    void OnStopSession();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RemoteControlPage( const RemoteControlPage& );            //!< Copy constructor
    RemoteControlPage& operator=( const RemoteControlPage& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Config& config_;
    QLineEdit* host_;
    QSpinBox* port_;
    ExerciseList* exercises_;
    ExerciseList* runningExercises_;
    const frontend::Exercise_ABC* exercise_;
    const frontend::Exercise_ABC* runningExercise_;
    std::auto_ptr< frontend::ExerciseFilter_ABC > filter_;
    std::auto_ptr< frontend::ExerciseFilter_ABC > runningFilter_;
    //@}
};

#endif // __RemoteControlPage_h_
