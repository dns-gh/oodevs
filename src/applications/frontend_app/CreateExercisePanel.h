// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateExercisePanel_h_
#define __CreateExercisePanel_h_

#include "Panel_ABC.h"
#include <Qt3Support/q3action.h>

namespace tools
{
    class GeneralConfig;
}

class InfoBubble;
class QLabel;
class QLineEdit;
class Q3ListBox;
class QPushButton;

// =============================================================================
/** @class  CreateExercisePanel
    @brief  CreateExercisePanel
*/
// Created: SBO 2007-10-04
// =============================================================================
class CreateExercisePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CreateExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context );
    virtual ~CreateExercisePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void CreateExercise();
    void ModelSelected();
    void NameChanged( const QString& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateExercisePanel( const CreateExercisePanel& );            //!< Copy constructor
    CreateExercisePanel& operator=( const CreateExercisePanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QStringList existingExercises_;
    QLineEdit* name_;
    Q3ListBox* modelList_;
    QLabel* physicalLabel_;
    Q3ListBox* physicalList_;
    Q3ListBox* terrainList_;
    QPushButton* okay_;
    InfoBubble* bubble_;
    //@}
};

#endif // __CreateExercisePanel_h_
