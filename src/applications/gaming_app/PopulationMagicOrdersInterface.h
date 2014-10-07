// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationMagicOrdersInterface_h_
#define __PopulationMagicOrdersInterface_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Population_ABC;
    class Profile_ABC;
    class Time_ABC;
    class DecisionalModel;
}

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class GlSelector;
    class LocationCreator;
    class ParametersLayer;
}

class StaticModel;

// =============================================================================
/** @class  PopulationMagicOrdersInterface
    @brief  PopulationMagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class PopulationMagicOrdersInterface : public QObject
                                     , public tools::Observer_ABC
                                     , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                                     , public gui::ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PopulationMagicOrdersInterface( QWidget* parent,
                                             kernel::Controllers& controllers,
                                             actions::ActionsModel& actionsModel,
                                             const StaticModel& staticModel,
                                             const kernel::Time_ABC& simulation,
                                             const std::shared_ptr< gui::ParametersLayer >& layer,
                                             const kernel::Profile_ABC& profile,
                                             gui::GlSelector& selector );
    virtual ~PopulationMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    virtual void Handle( kernel::Location_ABC& location );
    //@}

private slots:
    //! @name Slots
    //@{
    void Move();
    void KillAllPopulation();
    void ChangeArmedIndividuals();
    void ChangePopulationAttitude( int index );
    void ReloadBrain(QAction*);
    void ActivateBrainDebug();
    void DeactivateBrainDebug();
    //@}

private:
    //! @name Helpers
    //@{
    int AddMagic( const QString& label, const char* slot, kernel::ContextMenu* menu );
    void AddValuedMagic( kernel::ContextMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot );
    void AddReloadBrainMenu( QMenu* parent, const tools::StringResolver< kernel::DecisionalModel >& models,
        const std::string& currentModel, const std::string& defaultModel);
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    gui::GlSelector& selector_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    bool magicMove_;
    gui::LocationCreator* magicMoveLocation_;
    //@}
};

#endif // __PopulationMagicOrdersInterface_h_
