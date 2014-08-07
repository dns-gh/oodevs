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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Positions.h"
#include <tools/ElementObserver_ABC.h>
#ifndef Q_MOC_RUN
#include <boost/lexical_cast.hpp>
#endif
#include <xeumeuleu/xml.hpp>

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
             , public tools::Observer_ABC
             , public tools::ElementObserver_ABC< ConcreteEntity >
{
public:
    //! @name Functors
    //@{
    typedef std::function< void ( const int& ) > T_Setter;
    //@}

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
    template< typename M >
    void CommitTo( M& message ) const
    {
        // $$$$ FHD 2009-10-13: qt moc issues syntax error if not in class declaration
        if( GetValue() )
            message.set_id( GetValue()->GetId() );
    }
    void CommitTo( T_Setter setter ) const;
    virtual bool IsSet() const;
    virtual geometry::Point2f GetPosition() const;
    //@}

private:
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
    SetValue( 0 );
    Set( false );
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
// Created: FHD 2009-09-30
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Entity< ConcreteEntity >::CommitTo( T_Setter setter ) const
{
    if( GetValue() )
        setter( GetValue()->GetId() );
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

template< typename ConcreteEntity >
geometry::Point2f Entity< ConcreteEntity >::GetPosition() const
{
    if( auto entity = GetValue() )
        if( auto positions = entity->Retrieve< kernel::Positions >() )
            return positions->GetPosition();
    return Parameter< const ConcreteEntity* >::GetPosition();
}

    }
}

#endif // __ActionParameterEntity_h_
