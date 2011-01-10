// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMagicOrdersInterfaceProxy_h_
#define __ObjectMagicOrdersInterfaceProxy_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"


namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class Object_ABC;
    class Profile_ABC;
    class Time_ABC;
}

class StaticModel;
class MagicOrdersInterface_ABC;

// =============================================================================
/** @class  ObjectMagicOrdersInterfaceProxy
    @brief  ObjectMagicOrdersInterfaceProxy
*/
// Created: SBO 2007-05-04
// =============================================================================
class ObjectMagicOrdersInterfaceProxy : public tools::Observer_ABC
                                      , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMagicOrdersInterfaceProxy( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~ObjectMagicOrdersInterfaceProxy();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectMagicOrdersInterfaceProxy( const ObjectMagicOrdersInterfaceProxy& );            //!< Copy constructor
    ObjectMagicOrdersInterfaceProxy& operator=( const ObjectMagicOrdersInterfaceProxy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< MagicOrdersInterface_ABC > magicOrdersStrategy_;
    //@}
};

#endif // __ObjectMagicOrdersInterfaceProxy_h_
