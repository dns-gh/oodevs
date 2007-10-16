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

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class ParametersLayer;
    class SymbolIcons;
    class ColorStrategy_ABC;
}

class StaticModel;
class Publisher_ABC;
class ObjectCreationPanel;

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
    CreationPanels( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel, gui::ItemFactory_ABC& factory, Publisher_ABC& publisher, gui::ParametersLayer& paramLayer, kernel::GlTools_ABC& tools, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~CreationPanels();
    //@}

    //! @name Operations
    //@{
    void Draw( kernel::Viewport_ABC& viewport ) const;
    //@}

private:
    //! @name Member data
    //@{
    ObjectCreationPanel* objectCreationPanel_;
    //@}
};

#endif // __CreationPanels_h_
