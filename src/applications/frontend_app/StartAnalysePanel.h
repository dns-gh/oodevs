// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartAnalysePanel_h_
#define __StartAnalysePanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QSpinBox;
class QListBox;
class InfoBubble;
class GameConfigPanel;

// =============================================================================
/** @class  StartAnalysePanel
    @brief  StartAnalysePanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class StartAnalysePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             StartAnalysePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
    virtual ~StartAnalysePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void ExerciseSelected();
    void StartReplay();
    void ReplaySelected();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartAnalysePanel( const StartAnalysePanel& );            //!< Copy constructor
    StartAnalysePanel& operator=( const StartAnalysePanel& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QListBox* exercises_;
    QListBox* replays_;
    QPushButton* okay_;
    InfoBubble* bubble_;
    QSpinBox* exerciseNumber_;
    GameConfigPanel* configPanel_;
    //@}
};

#endif // __StartAnalysePanel_h_
