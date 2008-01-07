// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __RestartExercisePanel_h_
#define __RestartExercisePanel_h_

#include "StartExercisePanel.h"

// =============================================================================
/** @class  RestartExercisePanel
    @brief  RestartExercisePanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class RestartExercisePanel : public StartExercisePanel
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             RestartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
    virtual ~RestartExercisePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void StartExercise();
    virtual void OnTimer();
    virtual void ExerciseSelected();
    void SessionSelected();
    void CheckpointSelected();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RestartExercisePanel( const RestartExercisePanel& );            //!< Copy constructor
    RestartExercisePanel& operator=( const RestartExercisePanel& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    QListBox* sessionList_;
    QListBox* checkpointList_;
    //@}
};

#endif // __RestartExercisePanel_h_
