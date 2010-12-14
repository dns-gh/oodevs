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
#include "protocol/ServerPublisher_ABC.h"

namespace sword
{
    class MagicActionUpdateObject;
}

namespace actions
{
    class ActionsModel;

namespace parameters
{
    class ParameterList;
}
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Object_ABC;
    class Profile_ABC;
    class Time_ABC;
}

class StaticModel;

// =============================================================================
/** @class  ObjectMagicOrdersInterface
    @brief  ObjectMagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class ObjectMagicOrdersInterface : public QObject
                                 , public tools::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~ObjectMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void BuildObject();
    void DestroyObject();
    void MineObject();
    void SweepMineObject();
    void ActivateReservedObstacle();
    void DeactivateReservedObstacle();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectMagicOrdersInterface( const ObjectMagicOrdersInterface& );            //!< Copy constructor
    ObjectMagicOrdersInterface& operator=( const ObjectMagicOrdersInterface& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void SendObjectUpdateMagic( actions::parameters::ParameterList& attribute );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
