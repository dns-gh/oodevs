// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartAnalysisPanel_h_
#define __StartAnalysisPanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QSpinBox;
class QListBox;
class InfoBubble;

// =============================================================================
/** @class  StartAnalysisPanel
    @brief  StartAnalysisPanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class StartAnalysisPanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             StartAnalysisPanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
    virtual ~StartAnalysisPanel();
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
    StartAnalysisPanel( const StartAnalysisPanel& );            //!< Copy constructor
    StartAnalysisPanel& operator=( const StartAnalysisPanel& ); //!< Assignment operator
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
    //@}
};

#endif // __StartAnalysisPanel_h_
