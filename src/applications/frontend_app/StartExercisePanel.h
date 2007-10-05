// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartExercisePanel_h_
#define __StartExercisePanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QListBox;

// =============================================================================
/** @class  StartExercisePanel
    @brief  StartExercisePanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class StartExercisePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             StartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
    virtual ~StartExercisePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void StartExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartExercisePanel( const StartExercisePanel& );            //!< Copy constructor
    StartExercisePanel& operator=( const StartExercisePanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QListBox* list_;
    //@}
};

#endif // __StartExercisePanel_h_
