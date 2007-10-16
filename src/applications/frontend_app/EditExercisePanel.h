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

namespace tools
{
    class GeneralConfig;
}
class QListBox;
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
             EditExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
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
    QListBox* list_;
    InfoBubble* bubble_;
    //@}
};

#endif // __EditExercisePanel_h_
