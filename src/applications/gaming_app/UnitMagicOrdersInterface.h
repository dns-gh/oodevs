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
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Profile_ABC;
    class Team_ABC;
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

namespace sword
{
    enum UnitMagicAction_Type;
}

class StaticModel;

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
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitMagicOrdersInterface( QWidget* parent,
                                       kernel::Controllers& controllers,
                                       actions::ActionsModel& actionsModel,
                                       const StaticModel& staticModel,
                                       const kernel::Time_ABC& simulation,
                                       const std::shared_ptr< gui::ParametersLayer >& layer,
                                       const kernel::Profile_ABC& profile,
                                       gui::GlSelector& selector );
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
    void DeleteUnit();
    void Move();
    void RecoverHumanTransporters();
    void SurrenderTo( int );
    void ReloadBrain( QAction* );
    void ActivateBrainDebug();
    void DeactivateBrainDebug();
    void FinishLogisticHandlings();
    void Engage();
    void Disengage();
    //@}

private:
    //! @name Helpers
    //@{
    void AddMagic( const QString& label, sword::UnitMagicAction_Type id, kernel::ContextMenu* menu );
    int  AddMagic( const QString& label, const char* slot, kernel::ContextMenu* menu );
    void ApplyOnHierarchy( const kernel::Entity_ABC& entity, sword::UnitMagicAction_Type id );
    void FillCommonOrders( kernel::ContextMenu* magicMenu );
    void CreateAndPublish( const std::string& actionType );

    void AddSurrenderMenu( kernel::ContextMenu* parent, const kernel::Entity_ABC& entity );
    void AddReloadBrainMenu( QMenu* parent, const tools::StringResolver< kernel::DecisionalModel >& models,
                             const std::string& currentModel, const std::string& defaultModel );
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
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    gui::GlSelector& selector_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    bool magicMove_;
    gui::LocationCreator* magicMoveLocation_;
    T_Teams teams_;
    //@}
};

#endif // __UnitMagicOrdersInterface_h_
