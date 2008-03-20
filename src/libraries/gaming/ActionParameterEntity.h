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

#include "ActionParameter.h"
#include "xeumeuleu/xml.h"
#pragma warning (push)
#pragma warning (disable : 4127 4511 4512 )
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

// =============================================================================
/** @class  ActionParameterEntity
    @brief  ActionParameterEntity
*/
// Created: SBO 2007-05-04
// =============================================================================
template< typename ConcreteEntity >
class ActionParameterEntity : public ActionParameter< const ConcreteEntity* >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterEntity( const kernel::OrderParameter& parameter );
             ActionParameterEntity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity );
    virtual ~ActionParameterEntity();
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
    ActionParameterEntity( const ActionParameterEntity& );            //!< Copy constructor
    ActionParameterEntity& operator=( const ActionParameterEntity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const kernel::OrderParameter& parameter )
    : ActionParameter< const ConcreteEntity* >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity )
    : ActionParameter< const ConcreteEntity* >( parameter, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::~ActionParameterEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntity< ConcreteEntity >::CommitTo( ASN1T_OID& oid ) const
{
    if( GetValue() )
        oid = GetValue()->GetId();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity::CommitTo
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntity< ConcreteEntity >::CommitTo( std::string& content ) const
{
    if( GetValue() )
        content += boost::lexical_cast< std::string >( GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity::Serialize
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntity< ConcreteEntity >::Serialize( xml::xostream& xos ) const
{
    ActionParameter< const ConcreteEntity* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool ActionParameterEntity< ConcreteEntity >::IsSet() const
{
    return GetValue() != 0;
}

#endif // __ActionParameterEntity_h_
