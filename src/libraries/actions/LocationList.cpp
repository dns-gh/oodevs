// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "LocationList.h"
#include "Location.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationList::LocationList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationList::LocationList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_LocationList& asn )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location %1" ).arg( i ).ascii(), "location", false ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
LocationList::LocationList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &LocationList::ReadLocation, converter );
}

// -----------------------------------------------------------------------------
// Name: LocationList destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationList::~LocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationList::ReadLocation
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LocationList::ReadLocation( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new Location( OrderParameter( xml::attribute< std::string >( xis, "name" ), "location", false ), converter, xis ) );
}

// -----------------------------------------------------------------------------
// Name: LocationList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void LocationList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_locationList;
    ASN1T_LocationList*& list = asn.value.u.locationList = new ASN1T_LocationList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_Location[list->n];
        CommitTo( *list );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void LocationList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.locationList )
        Clean( *asn.value.u.locationList );
    delete asn.value.u.locationList;
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_LocationList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Location& param )
        {
            if( current_ < asn_->n )
                param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_LocationList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: LocationList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void LocationList::CommitTo( ASN1T_LocationList& asn ) const
{
    asn.n = Count();
    asn.elem = asn.n ? new ASN1T_Location[ asn.n ] : 0;
    AsnSerializer serializer( asn );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_LocationList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Location& param )
        {
            if( current_ < asn_->n )
                param.Clean( asn_->elem[current_++] );
        }

        ASN1T_LocationList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: LocationList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void LocationList::Clean( ASN1T_LocationList& asn ) const
{
    if( asn.n )
    {
        AsnCleaner cleaner( asn );
        Accept( cleaner );
        delete[] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void LocationList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: LocationList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool LocationList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
