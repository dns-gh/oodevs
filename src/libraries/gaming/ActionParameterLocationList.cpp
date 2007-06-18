// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLocationList.h"
#include "ActionParameterLocation.h"
#include "ActionParameterVisitor_ABC.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocationList::ActionParameterLocationList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocationList::ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LocationList& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location %1" ).arg( i ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterLocationList::ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterLocationList::ReadLocation, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocationList::~ActionParameterLocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList::ReadLocation
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocationList::ReadLocation( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterLocation( converter, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLocationList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_locationList;
    ASN1T_LocationList*& list = asn.value.u.locationList = new ASN1T_LocationList();
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Location[list->n];
    CommitTo( *list );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLocationList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.locationList )
        Clean( *asn.value.u.locationList );
    delete asn.value.u.locationList;
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_LocationList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterLocation& param )
        {
            if( current_ < asn_->n )
                param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_LocationList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLocationList::CommitTo( ASN1T_LocationList& asn ) const
{
    AsnSerializer serializer( asn );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_LocationList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterLocation& param )
        {
            if( current_ < asn_->n )
                param.Clean( asn_->elem[current_++] );
        }

        ASN1T_LocationList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLocationList::Clean( ASN1T_LocationList& asn ) const
{
    if( asn.n )
    {
        AsnCleaner cleaner( asn );
        Accept( cleaner );
        delete[] asn.elem;
    }
}
