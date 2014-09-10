// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __EntityTreeView_h_
#define __EntityTreeView_h_

#include "EntityTreeView_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Team_ABC.h"

namespace kernel
{
    class Controllers;
    class Inhabitant_ABC;
    class Object_ABC;
    class Population_ABC;
    class UrbanObject_ABC;
    class Profile_ABC;
}

namespace gui
{
class ModelObserver_ABC;

// =============================================================================
/** @class  EntityTreeView
    @brief  EntityTreeView
*/
// Created: ABR 2012-08-10
// =============================================================================
template< typename Entity >
class EntityTreeView : public EntityTreeView_ABC
                     , public tools::ElementObserver_ABC< Entity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityTreeView( const QString& objectName,
                             kernel::Controllers& controllers,
                             const kernel::Profile_ABC& profile,
                             ModelObserver_ABC& modelObserver,
                             bool editable,
                             QWidget* parent = 0 );
    virtual ~EntityTreeView();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const Entity& entity );
    virtual void NotifyDeleted( const Entity& entity );
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const;
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual QStringList MimeTypes() const { return QStringList(); } // temp
    //@}

private:
    //! @name Member data
    //@{
    bool editable_;
    //@}
};

#include "EntityTreeView.inl"

typedef EntityTreeView< kernel::Inhabitant_ABC > InhabitantTreeView;
typedef EntityTreeView< kernel::Population_ABC > PopulationTreeView;

} //! namespace gui

#endif // __EntityTreeView_h_
