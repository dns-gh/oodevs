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
    class ItemFactory_ABC;
    class GlProxy;
    class ParametersLayer;
    class SymbolIcons;
    class WeatherLayer;
}

class StaticModel;
class Model;
class ObjectCreationPanel;
class InhabitantCreationPanel;

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
             CreationPanels( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel, const Model& model,
                             const tools::ExerciseConfig& config, gui::ItemFactory_ABC& factory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy,
                             gui::ParametersLayer& paramLayer, gui::WeatherLayer& weatherLayer, gui::GlProxy& glProxy );
    virtual ~CreationPanels();
    //@}

    //! @name Operations
    //@{
    ObjectCreationPanel& GetObjectCreationPanel() const;
    InhabitantCreationPanel& GetInhabitantCreationPanel() const;
    //@}

private:
    //! @name Member data
    //@{
    ObjectCreationPanel* objectCreationPanel_;
    InhabitantCreationPanel* inhabitantCreationPanel_;
    //@}
};

#endif // __CreationPanels_h_
