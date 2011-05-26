// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreationPanels_h_
#define __CreationPanels_h_

#include "clients_gui/Panels.h"

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ColorStrategy_ABC;
    class IntelligencesPanel;
    class ItemFactory_ABC;
    class SymbolIcons;
}

class StaticModel;

// =============================================================================
/** @class  CreationPanels
    @brief  CreationPanels
*/
// Created: SBO 2006-08-28
// =============================================================================
class CreationPanels : public gui::Panels
{
public:
    //! @name Constructors/Destructor
    //@{
             CreationPanels( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel, gui::ItemFactory_ABC& factory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~CreationPanels();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    //@}

private:
    //! @name Member data
    //@{
    gui::IntelligencesPanel* intelligencesPanel_;
    //@}
};

#endif // __CreationPanels_h_
