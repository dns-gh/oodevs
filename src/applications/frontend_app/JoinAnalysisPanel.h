// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __JoinAnalysisPanel_h_
#define __JoinAnalysisPanel_h_

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
/** @class  JoinAnalysisPanel
    @brief  JoinAnalysisPanel
*/
// Created: AGE 2007-10-05
// =============================================================================
class JoinAnalysisPanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             JoinAnalysisPanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context );
    virtual ~JoinAnalysisPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void StartExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    JoinAnalysisPanel( const JoinAnalysisPanel& );            //!< Copy constructor
    JoinAnalysisPanel& operator=( const JoinAnalysisPanel& ); //!< Assignment operator
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
    QSpinBox* exerciseNumber_;
    //@}
};

#endif // __JoinAnalysisPanel_h_
