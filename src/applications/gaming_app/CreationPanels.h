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
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class UnitsPanel;
    class ItemFactory_ABC;
    class ParametersLayer;
    class SymbolIcons;
    class ColorStrategy_ABC;
    class IntelligencesPanel;
}

class StaticModel;
class Publisher_ABC;
class ObjectCreationPanel;
class Simulation;

// =============================================================================
/** @class  CreationPanels
    @brief  CreationPanels
*/
// Created: SBO 2006-08-28
// =============================================================================
class CreationPanels : public gui::Panels
                     , public kernel::Observer_ABC
                     , public kernel::ElementObserver_ABC< Simulation >
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
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const Simulation& simu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    bool shown_;
    gui::UnitsPanel* units_;
    ObjectCreationPanel* objects_;
    gui::IntelligencesPanel* intel_;
    //@}
};

#endif // __CreationPanels_h_
