// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMagicOrdersInterface_h_
#define __ObjectMagicOrdersInterface_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Entity_ABC;
    class Object_ABC;
    class UrbanObject_ABC;
    class Profile_ABC;
}

// =============================================================================
/** @class  ObjectMagicOrdersInterface
    @brief  ObjectMagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class ObjectMagicOrdersInterface : public QObject
                                 , public tools::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Profile_ABC& profile );
    virtual ~ObjectMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::UrbanObject_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void BuildObject();
    void DestroyObject();
    void MineObject();
    void SweepMineObject();
    void ActivateObstacle();
    void DeactivateObstacle();
    void ChangeStructuralState();
    void ChangeThreshold();
    void DisableInfrastructure();
    void EnableInfrastructure();
    void Alert();
    void StopAlert();
    void Confine();
    void StopConfine();
    void Evacuate();
    void StopEvacuate();
    void ActivateUndergroundExit();
    void DeactivateUndergroundExit();
    void ChangeTrafficability();
    //@}

private:
    //! @name Helpers
    //@{
    int AddMagic( const QString& label, const char* slot, kernel::ContextMenu* menu );
    void AddIntValuedMagic( kernel::ContextMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot, unsigned int value );
    void AddDoubleValuedMagic( kernel::ContextMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot, double value );
    void DoMineObject( int quantity );
    void PublishActivation( const std::string& name, unsigned int id, bool activate );
    void DoActivateObstacle( bool activate );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
