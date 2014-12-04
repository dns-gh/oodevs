// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InfoDock_h_
#define __InfoDock_h_

#include "clients_gui/RichDockWidget.h"
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class KnowledgeConverter_ABC;
    class Profile_ABC;
}

namespace gui
{
    class DisplayExtractor;
    class EntitySymbols;
    class ItemFactory_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class InfoWidget;
class Model;
class Simulation;
class SimulationController;
class UnitStateDialog;

// =============================================================================
/** @class  InfoDock
    @brief  InfoDock
*/
// Created: SBO 2007-02-02
// =============================================================================
class InfoDock : public gui::RichDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoDock( QWidget* parent,
                       kernel::Controllers& controllers,
                       const kernel::Profile_ABC& profile,
                       const gui::EntitySymbols& icons,
                       gui::ItemFactory_ABC& itemFactory,
                       gui::DisplayExtractor& extractor,
                       const kernel::KnowledgeConverter_ABC& converter,
                       Model& model,
                       const Simulation& simulation,
                       SimulationController& simulationController,
                       UnitStateDialog& unitStateDialog );
    virtual ~InfoDock();
    //@}

    void Purge();

private:
    InfoWidget* widget_;
};

#endif // __InfoDock_h_
