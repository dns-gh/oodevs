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
ActionParameterLimaList::ActionParameterLimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& limas )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < limas.n; ++i )
        AddParameter( *new ActionParameterLima( OrderParameter( tools::translate( "ActionParameter", "Lima %1" ).arg( i + 1 ), "lima", false ), converter, limas.elem[i] ) );
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
void ActionParameterLimaList::CommitTo( ASN1T_OrderContext& asn ) const
{
    asn.limas.n = Count();;
    if( !asn.limas.n )
        return;
    asn.limas.elem = new ASN1T_LimaOrder[asn.limas.n];
    AsnSerializer serializer( asn.limas );
    Accept( serializer );
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
void ActionParameterLimaList::Clean( ASN1T_OrderContext& asn ) const
{
    if( asn.limas.n )
    {
        AsnCleaner cleaner( asn.limas );
        Accept( cleaner );
        delete[] asn.limas.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterLimaList::DisplayTooltip( const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( tools );
}
