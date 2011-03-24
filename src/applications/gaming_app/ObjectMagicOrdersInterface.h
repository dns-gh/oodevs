// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMagicOrdersInterface_h_
#define __ObjectMagicOrdersInterface_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>

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
    class Entity_ABC;
    class Object_ABC;
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
                                 , private boost::noncopyable
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
    void ChangeStructuralState();
    void ChangeThreshold();
    void Disable();
    void Enable();
    void Alert();
    void StopAlert();
    void Confine();
    void StopConfine();
    void Evacuate();
    void StopEvacuate();
    //@}

private:
    //! @name Helpers
    //@{
    int AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void AddValuedMagic( QPopupMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Object_ABC > selectedEntity_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
