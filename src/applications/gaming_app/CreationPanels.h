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
#include "tools/ElementObserver_ABC.h"

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class ModelLoaded;
    class ModelUnLoaded;
    class Time_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class DrawerPanel;
    class IntelligencesPanel;
    class ItemFactory_ABC;
    class ParametersLayer;
    class PopulationsPanel;
    class SymbolIcons;
    class UnitsPanel;
}

namespace actions
{
    class ActionsModel;
}

class DrawingsModel;
class FireCreationPanel;
class ObjectCreationPanel;
class StaticModel;
class WeatherCreationPanel;

// =============================================================================
/** @class  CreationPanels
    @brief  CreationPanels
*/
// Created: SBO 2006-08-28
// =============================================================================
class CreationPanels : public gui::Panels
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             CreationPanels( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel, gui::ItemFactory_ABC& factory, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, gui::ParametersLayer& paramLayer, kernel::GlTools_ABC& tools, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy, DrawingsModel& drawings, const tools::ExerciseConfig& config );
    virtual ~CreationPanels();
    //@}

    //! @name Operations
    //@{
    void Draw( kernel::Viewport_ABC& viewport ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::UnitsPanel* units_;
    ObjectCreationPanel* objects_;
    gui::IntelligencesPanel* intel_;
    WeatherCreationPanel* weather_;
    gui::PopulationsPanel* crowds_;
    gui::DrawerPanel* drawings_;
    FireCreationPanel* fires_;
    //@}
};

#endif // __CreationPanels_h_
