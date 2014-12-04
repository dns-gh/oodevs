// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoWidget_h_
#define __InfoWidget_h_

namespace kernel
{
    class Controllers;
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

class InfoConflictsTab;
class InfoMissionsTab;
class InfoReportsTab;
class Model;
class Simulation;
class SimulationController;
class UnitStateDialog;

// =============================================================================
/** @class  InfoWidget
    @brief  InfoWidget
*/
// Created: SBO 2007-02-02
// =============================================================================
class InfoWidget : public Q3HBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InfoWidget( QWidget* parent,
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
    virtual ~InfoWidget();
    //@}

    void Purge();

private:
    InfoReportsTab* reports_;
    InfoConflictsTab* conflicts_;
    InfoMissionsTab* missions_;
};

#endif // __InfoWidget_h_
