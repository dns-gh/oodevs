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

namespace frontend
{
    class PluginConfig_ABC;
}

namespace tools
{
    class GeneralConfig;
}

class InfoBubble;
class GameConfigPanel;
class QGroupBox;
class QLineEdit;
class QListBox;
class QPushButton;
class QSpinBox;
class QTextEdit;

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
             StartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config, ActionsContext& context, const char* name = 0 );
    virtual ~StartExercisePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void StartExercise();
    virtual void OnTimer();
    virtual void ExerciseSelected();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartExercisePanel( const StartExercisePanel& );            //!< Copy constructor
    StartExercisePanel& operator=( const StartExercisePanel& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    QString BuildSessionDirectory();
    template< typename T >
    void AddPlugin( QTabWidget* tabs );
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< frontend::PluginConfig_ABC* > T_Plugins;
    //@}

protected:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* listBox_;
    QListBox* list_;
    GameConfigPanel* configPanel_;
    QLineEdit* sessionName_;
    QTextEdit* sessionComment_;
    QSpinBox* exerciseNumber_;
    InfoBubble* bubble_;
    QPushButton* okay_;
    std::string session_;
    T_Plugins plugins_;
    //@}
};

#endif // __StartExercisePanel_h_
