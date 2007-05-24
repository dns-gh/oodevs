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

#include "game_asn/Asn.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Profile_ABC;
    class Object_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  ObjectMagicOrdersInterface
    @brief  ObjectMagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class ObjectMagicOrdersInterface : public QObject
                                 , public kernel::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
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
    void SendObjectMagic( ASN1T_MagicActionUpdateObject& asn );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
