// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterEntity_h_
#define __ActionParameterEntity_h_

#include "Parameter.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.h>
#pragma warning (push)
#pragma warning (disable : 4127 4511 4512 )
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Entity
    @brief  Entity
*/
// Created: SBO 2007-05-04
// =============================================================================
template< typename ConcreteEntity >
class Entity : public Parameter< const ConcreteEntity* >
             , public kernel::Observer_ABC
             , public kernel::ElementObserver_ABC< ConcreteEntity >
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Entity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity, kernel::Controller& controller );
    virtual ~Entity();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( ASN1T_OID& oid ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity( const Entity& );            //!< Copy constructor
    Entity& operator=( const Entity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const ConcreteEntity& );
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Entity constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Entity< ConcreteEntity >::Entity( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Parameter< const ConcreteEntity* >( parameter )
    , controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Entity constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Entity< ConcreteEntity >::Entity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity, kernel::Controller& controller )
    : Parameter< const ConcreteEntity* >( parameter, entity )
    , controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Entity destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Entity< ConcreteEntity >::~Entity()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Entity::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Entity< ConcreteEntity >::CommitTo( ASN1T_OID& oid ) const
{
    if( GetValue() )
        oid = GetValue()->GetId();
}

// -----------------------------------------------------------------------------
// Name: Entity::CommitTo
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Entity< ConcreteEntity >::CommitTo( std::string& content ) const
{
    if( GetValue() )
        content += boost::lexical_cast< std::string >( GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: Entity::NotifyDeleted
// Created: SBO 2008-10-09
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Entity< ConcreteEntity >::NotifyDeleted( const ConcreteEntity& entity )
{
    if( &entity == GetValue() )
        SetValue( 0 );
}

// -----------------------------------------------------------------------------
// Name: Entity::Serialize
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Entity< ConcreteEntity >::Serialize( xml::xostream& xos ) const
{
    Parameter< const ConcreteEntity* >::Serialize( xos );
    if( GetValue() )
        xos << xml::attribute( "value", GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: Entity::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool Entity< ConcreteEntity >::IsSet() const
{
    return GetValue() != 0;
}

    }
}

#endif // __ActionParameterEntity_h_
