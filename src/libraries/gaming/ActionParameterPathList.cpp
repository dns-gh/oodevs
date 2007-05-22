// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPathList.h"
#include "ActionParameterPath.h"
#include "ActionParameterVisitor_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListItineraire& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterPath( tools::translate( "ActionParameter", "Route %1" ).arg( i ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterPathList::ReadPath, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::~ActionParameterPathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::ReadPath
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterPathList::ReadPath( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterPath( converter, xis ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ListItineraire& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterPath& param )
        {
            if( current_ < asn_->n )
                param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_ListItineraire* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPathList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listItineraire;
    ASN1T_ListItineraire*& list = asn.value.u.listItineraire = new ASN1T_ListItineraire();
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Itineraire[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_ListItineraire& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterPath& param )
        {
            if( current_ < asn_->n )
                param.Clean( asn_->elem[current_++] );
        }

        ASN1T_ListItineraire* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPathList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listItineraire )
    {
        AsnCleaner cleaner( *asn.value.u.listItineraire );
        Accept( cleaner );
    }
    delete[] asn.value.u.listItineraire;
}
