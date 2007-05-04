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
             ActionParameterEntity( const QString& name, const ConcreteEntity* entity );
    virtual ~ActionParameterEntity();
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
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const QString& name, const ConcreteEntity* entity )
    : ActionParameter< const ConcreteEntity* >( name, entity )
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
// Name: ActionParameterEntity::Serialize
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntity< ConcreteEntity >::Serialize( xml::xostream& xos ) const
{
    ActionParameter< const ConcreteEntity* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetId() );
}

#endif // __ActionParameterEntity_h_
