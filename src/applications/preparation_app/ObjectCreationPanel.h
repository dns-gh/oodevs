// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectCreationPanel_h_
#define __ObjectCreationPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class Team_ABC;
}

namespace gui
{
    class GLView_ABC;
    class ObjectPrototype_ABC;
    class PanelStack_ABC;
    class ParametersLayer;
    class Viewport_ABC;
}

class StaticModel;
class ObjectsModel;
class UrbanModel;
class WeatherModel;

// =============================================================================
/** @class  ObjectCreationPanel
    @brief  Object creation panel
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectCreationPanel : public gui::InfoPanel_ABC
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectCreationPanel( QWidget* parent,
                                  gui::PanelStack_ABC& panel,
                                  kernel::Controllers& controllers,
                                  const StaticModel& model,
                                  ObjectsModel& objectsModel,
                                  const UrbanModel& urbanModel,
                                  const WeatherModel& weatherModel,
                                  const kernel::Team_ABC& noSideTeam,
                                  const std::shared_ptr< gui::ParametersLayer >& layer,
                                  const gui::GLView_ABC& tools,
                                  const tools::GeneralConfig& config );
    virtual ~ObjectCreationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( gui::Viewport_ABC& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const gui::GLView_ABC& tools_;
    gui::ObjectPrototype_ABC* created_;
    //@}
};

#endif // __ObjectCreationPanel_h_
