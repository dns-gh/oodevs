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
#include <Qt3Support/q3action.h>

namespace tools
{
    class GeneralConfig;
}
class Q3ListBox;
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
             JoinExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context );
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
    Q3ListBox* list_;
    Q3ListBox* sessionList_;
    InfoBubble* bubble_;
    //@}
};

#endif // __JoinExercisePanel_h_
