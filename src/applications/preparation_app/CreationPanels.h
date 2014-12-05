// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    class Profile_ABC;
}

namespace gui
{
    class ColorController_ABC;
    class ColorStrategy_ABC;
    class GLView_ABC;
    class ParametersLayer;
    class SymbolIcons;
    class WeatherLayer;
}

class GhostsPanel;
class InhabitantCreationPanel;
class Model;
class ObjectCreationPanel;
class StaticModel;
class WeatherPanel;

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
             CreationPanels( QWidget* parent,
                             kernel::Controllers& controllers,
                             const StaticModel& staticModel,
                             const Model& model,
                             const tools::ExerciseConfig& config,
                             gui::SymbolIcons& icons,
                             gui::ColorStrategy_ABC& colorStrategy,
                             const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                             const std::shared_ptr< gui::WeatherLayer >& weatherLayer,
                             gui::GLView_ABC& glProxy,
                             gui::ColorController_ABC& colorController,
                             const kernel::Profile_ABC& profile );
    virtual ~CreationPanels();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Purge();
    ObjectCreationPanel& GetObjectCreationPanel() const;
    InhabitantCreationPanel& GetInhabitantCreationPanel() const;
    //@}

private:
    //! @name Member data
    //@{
    ObjectCreationPanel*     objectCreationPanel_;
    InhabitantCreationPanel* inhabitantCreationPanel_;
    GhostsPanel*             ghostPanel_;
    WeatherPanel*            weatherPanel_;
    //@}
};

#endif // __CreationPanels_h_
