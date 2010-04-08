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
#include "clients_kernel/LocationVisitor_ABC.h"
#include "protocol/ServerPublisher_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Profile_ABC;
    class Population_ABC;
}

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class LocationCreator;
    class ParametersLayer;
}

class StaticModel;
class Publisher_ABC;
class ActionPublisher;
class Simulation;

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
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile );
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
    void KillSomePopulation();
    void ResurectSomePopulation();
    void ChangePopulationAttitude( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationMagicOrdersInterface( const PopulationMagicOrdersInterface& );            //!< Copy constructor
    PopulationMagicOrdersInterface& operator=( const PopulationMagicOrdersInterface& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void AddValuedMagic( QPopupMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    ActionPublisher& actionPublisher_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const Simulation& simulation_; 
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    bool magicMove_;
    gui::LocationCreator* magicMoveLocation_;
    //@}
};

#endif // __PopulationMagicOrdersInterface_h_
