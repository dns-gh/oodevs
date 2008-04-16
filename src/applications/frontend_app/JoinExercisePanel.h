// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __JoinExercisePanel_h_
#define __JoinExercisePanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QListBox;
class InfoBubble;
class QSpinBox;

// =============================================================================
/** @class  JoinExercisePanel
    @brief  JoinExercisePanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class JoinExercisePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             JoinExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config, ActionsContext& context );
    virtual ~JoinExercisePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void StartExercise();
    void ExerciseSelected();
    void SessionSelected();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    JoinExercisePanel( const JoinExercisePanel& );            //!< Copy constructor
    JoinExercisePanel& operator=( const JoinExercisePanel& ); //!< Assignment operator
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
    QListBox* sessionList_;
    InfoBubble* bubble_;
    //@}
};

#endif // __JoinExercisePanel_h_
