// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RestartExercisePanel_h_
#define __RestartExercisePanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QListBox;
class InfoBubble;

// =============================================================================
/** @class  RestartExercisePanel
    @brief  RestartExercisePanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class RestartExercisePanel : public Panel_ABC
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
    void StartExercise();
    void ExerciseSelected();
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
    const tools::GeneralConfig& config_;
    QListBox* list_;
    QListBox* checkpointList_;
    InfoBubble* bubble_;
    QPushButton* okay_;
    //@}
};

#endif // __RestartExercisePanel_h_
