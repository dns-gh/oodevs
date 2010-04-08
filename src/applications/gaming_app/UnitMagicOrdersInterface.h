// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitMagicOrdersInterface_h_
#define __UnitMagicOrdersInterface_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "protocol/ServerPublisher_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Profile_ABC;
    class Team_ABC;
    class Formation_ABC;
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
/** @class  UnitMagicOrdersInterface
    @brief  UnitMagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class UnitMagicOrdersInterface : public QObject
                               , public tools::Observer_ABC
                               , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                               , public tools::ElementObserver_ABC< kernel::Team_ABC >
                               , public gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UnitMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile );
    virtual ~UnitMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void Handle( kernel::Location_ABC& location );
    //@}

private slots:
    //! @name Slots
    //@{
    void Magic( int );
    void DestroyComponent();
    void Move();
    void RecoverHumanTransporters();
    void SurrenderTo( int );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UnitMagicOrdersInterface( const UnitMagicOrdersInterface& );            //!< Copy constructor
    UnitMagicOrdersInterface& operator=( const UnitMagicOrdersInterface& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddMagic( const QString& label, int id,           QPopupMenu* menu );
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void ApplyOnHierarchy( const kernel::Entity_ABC& entity, int id );
    void FillCommonOrders( QPopupMenu* magicMenu );

    void AddSurrenderMenu( QPopupMenu* parent, const kernel::Entity_ABC& entity );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Team_ABC* > T_Teams;
    typedef T_Teams::const_iterator              CIT_Teams;
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
    T_Teams teams_;
    //@}
};

#endif // __UnitMagicOrdersInterface_h_
