// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterAction_h_
#define __AfterAction_h_

#include "clients_gui/RichDockWidget.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class ExerciseConfig;
}

class AfterActionModel;
class IndicatorPlotFactory;

// =============================================================================
/** @class  AfterAction
    @brief  AfterAction
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterAction : public gui::RichDockWidget
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             AfterAction( QWidget* parent, kernel::Controllers& controllers, const tools::ExerciseConfig& config, AfterActionModel& model,
                          IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder );
    virtual ~AfterAction();
    //@}

private slots:
    //! @name 
    //@{
    void OnLoad();
    void OnSave();
    //@}

private:
    //! @Member data
    //@{
    const tools::ExerciseConfig& config_;
    AfterActionModel& model_;
    //@}
};

#endif // __AfterAction_h_
