// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EditExercisePanel_h_
#define __EditExercisePanel_h_

#include "Panel_ABC.h"
#include <Qt3Support/q3action.h>

namespace tools
{
    class GeneralConfig;
}
class Q3ListBox;
class InfoBubble;

// =============================================================================
/** @class  EditExercisePanel
    @brief  EditExercisePanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class EditExercisePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EditExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context );
    virtual ~EditExercisePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void EditExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EditExercisePanel( const EditExercisePanel& );            //!< Copy constructor
    EditExercisePanel& operator=( const EditExercisePanel& ); //!< Assignment operator
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
    InfoBubble* bubble_;
    //@}
};

#endif // __EditExercisePanel_h_
