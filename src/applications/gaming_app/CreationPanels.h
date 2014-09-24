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
#include "clients_gui/RichDockWidget.h"
#include <tools/ElementObserver_ABC.h>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
    class Time_ABC;
    class Profile_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class DrawerPanel;
    class GlTools_ABC;
    class ParametersLayer;
    class PopulationsPanel;
    class SymbolIcons;
    class UnitsPanel;
    class Viewport_ABC;
}

class FireCreationPanel;
class ObjectCreationPanel;
class Model;
class StaticModel;
class WeatherLayer;
class WeatherPanel;

// =============================================================================
/** @class  CreationPanels
    @brief  CreationPanels
*/
// Created: SBO 2006-08-28
// =============================================================================
class CreationPanels : public gui::RichDockWidget
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                     , public tools::ElementObserver_ABC< kernel::Profile_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    CreationPanels( QWidget* parent,
                    kernel::Controllers& controllers,
                    const StaticModel& staticModel,
                    const Model& model,
                    const kernel::Time_ABC& simulation,
                    const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                    const std::shared_ptr< ::WeatherLayer >& weatherLayer,
                    gui::GlTools_ABC& tools,
                    gui::SymbolIcons& icons,
                    gui::ColorStrategy_ABC& colorStrategy,
                    const tools::ExerciseConfig& config );
    virtual ~CreationPanels();
    //@}

    //! @name Operations
    //@{
    void Draw( gui::Viewport_ABC& viewport ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& model );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    //@}

    //! @name Helpers
    //@{
    void AddPanels();
    void RemovePanels();
    //@}

private:
    //! @name Member data
    //@{
    gui::Panels*             panels_;
    gui::UnitsPanel*         units_;
    ObjectCreationPanel*     objects_;
    WeatherPanel*            weather_;
    gui::PopulationsPanel*   crowds_;
    gui::DrawerPanel*        drawings_;
    FireCreationPanel*       fires_;
    //@}
};

#endif // __CreationPanels_h_
