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
    class GlProxy;
    class ParametersLayer;
    class SymbolIcons;
    class WeatherLayer;
}

class ColorController;
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
                             gui::GlProxy& glProxy,
                             ColorController& colorController );
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
