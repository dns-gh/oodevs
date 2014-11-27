// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DrawerPanel_h_
#define __DrawerPanel_h_

#include "clients_gui/DrawerPanel.h"

// =============================================================================
/** @class  DrawerPanel
    @brief  DrawerPanel
*/
// Created: JSR 2014-11-26
// =============================================================================
class DrawerPanel : public gui::DrawerPanel
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawerPanel( QWidget* parent,
                          gui::PanelStack_ABC& panel,
                          const std::shared_ptr< gui::ParametersLayer >& layer,
                          kernel::Controllers& controllers,
                          gui::DrawerModel& model,
                          gui::DrawingTypes& types,
                          const tools::ExerciseConfig& config,
                          const kernel::Profile_ABC& profile );
    virtual ~DrawerPanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoClear() const;
    //@}
};

#endif // __DrawerPanel_h_
