// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLocationList_h_
#define __ActionParameterLocationList_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"
#include "ActionParameterLocation.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  ActionParameterLocationList
    @brief  ActionParameterLocationList
*/
// Created: SBO 2007-04-20
// =============================================================================
template< typename List >
class ActionParameterLocationList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLocationList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const List& asn );
             ActionParameterLocationList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const List& asn, const kernel::Entity_ABC& entity );
    virtual ~ActionParameterLocationList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLocationList( const ActionParameterLocationList& );            //!< Copy constructor
    ActionParameterLocationList& operator=( const ActionParameterLocationList& ); //!< Assignment operator
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
template< typename List >
ActionParameterLocationList< List >::ActionParameterLocationList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const List& asn )
    : ActionParameter< QString >( name, false )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        Register( i, *new ActionParameterLocation( "", converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
template< typename List >
ActionParameterLocationList< List >::ActionParameterLocationList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const List& asn, const kernel::Entity_ABC& entity )
    : ActionParameter< QString >( name, false )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        Register( i, *new ActionParameterLocation( "", converter, asn.elem[i], entity ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList destructor
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
template< typename List >
ActionParameterLocationList< List >::~ActionParameterLocationList()
{
    // NOTHING
}

#endif // __ActionParameterLocationList_h_
