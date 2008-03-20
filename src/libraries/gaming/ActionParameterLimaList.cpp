// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimaList.h"
#include "ActionParameterLima.h"
#include "ActionParameterVisitor_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterLima( OrderParameter( tools::translate( "ActionParameter", "Phase line %1" ).arg( i ).ascii(), "lima", true ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterLimaList::ReadLima, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::~ActionParameterLimaList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::ReadLima
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterLimaList::ReadLima( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterLima( converter, xis ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_LimasOrder& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterLima& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }
        ASN1T_LimasOrder* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLimaList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_limasOrder;
    ASN1T_LimasOrder*& list = asn.value.u.limasOrder = new ASN1T_LimasOrder();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_LimaOrder[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_LimasOrder& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterLima& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
        ASN1T_LimasOrder* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLimaList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.limasOrder )
    {
        AsnCleaner cleaner( *asn.value.u.limasOrder );
        Accept( cleaner );
        delete[] asn.value.u.limasOrder;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterLimaList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ActionParameterLimaList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
