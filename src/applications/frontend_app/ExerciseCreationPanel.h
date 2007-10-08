// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseCreationPanel_h_
#define __ExerciseCreationPanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QLineEdit;
class QListBox;
class InfoBubble;

// =============================================================================
/** @class  ExerciseCreationPanel
    @brief  ExerciseCreationPanel
*/
// Created: SBO 2007-10-04
// =============================================================================
class ExerciseCreationPanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseCreationPanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
    virtual ~ExerciseCreationPanel();
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
    ExerciseCreationPanel( const ExerciseCreationPanel& );            //!< Copy constructor
    ExerciseCreationPanel& operator=( const ExerciseCreationPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QStringList existingExercises_;
    QLineEdit* name_;
    QListBox* modelList_;
    QLabel* physicalLabel_;
    QListBox* physicalList_;
    QListBox* terrainList_;
    QPushButton* okay_;
    InfoBubble* bubble_;
    //@}
};

#endif // __ExerciseCreationPanel_h_
