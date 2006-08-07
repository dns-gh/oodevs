// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchBox_h_
#define __EntitySearchBox_h_

#include "EntitySearchBox_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/ActionController.h"

// =============================================================================
/** @class  EntitySearchBox
    @brief  EntitySearchBox
*/
// Created: SBO 2006-04-20
// =============================================================================
template< typename Entity >
class EntitySearchBox : public EntitySearchBox_ABC
                      , public Observer_ABC
                      , public ElementObserver_ABC< Entity >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntitySearchBox( QWidget* pParent, Controllers& controllers );
    virtual ~EntitySearchBox();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntitySearchBox( const EntitySearchBox& );            //!< Copy constructor
    EntitySearchBox& operator=( const EntitySearchBox& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Entity& );
    virtual void NotifyDeleted( const Entity& );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    //@}
};

namespace
{
    template< typename Entity >
    class Searchable : public EntitySearchBox_ABC::SearchableItem_ABC
    {
    public:
        Searchable( Controllers& controllers, const Entity& entity )
            : SearchableItem_ABC( entity.GetId() )
            , controllers_( &controllers )
            , entity_( &entity )
            , name_( entity.GetName().c_str() + QString::number( entity.GetId() ) )
        {
            name_ = name_.lower();
        };
        virtual ~Searchable()
        {};
        virtual bool Matches( const QString& input ) const {
            return name_.find( input ) != -1;
        };
        virtual void Activate() 
        {
            controllers_->actions_.Select( *entity_ );
            controllers_->actions_.Activate( *entity_ );
        };
    private:
        Controllers* controllers_;
        const Entity* entity_;
        QString name_;
    };
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
EntitySearchBox< Entity >::EntitySearchBox( QWidget* pParent, Controllers& controllers )
    : EntitySearchBox_ABC( pParent )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
EntitySearchBox< Entity >::~EntitySearchBox()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::NotifyCreated( const Entity& entity )
{
    AddItem( *new Searchable< Entity >( controllers_, entity ) );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::NotifyDeleted( const Entity& entity )
{
    RemoveItem( entity.GetId() );
}


#endif // __EntitySearchBox_h_
