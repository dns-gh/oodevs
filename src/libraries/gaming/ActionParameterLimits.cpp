// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimits.h"
#include "ActionParameterLimit.h"
#include "ActionParameterVisitor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLimits::ActionParameterLimits( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLimits::ActionParameterLimits( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Line& line1, const ASN1T_Line& line2 )
    : ActionParameter< QString >( parameter )
{
    AddParameter( *new ActionParameterLimit( OrderParameter( tools::translate( "ActionParameterLimits", "Limit 1" ), "limit", false, true ), converter, line1 ) );
    AddParameter( *new ActionParameterLimit( OrderParameter( tools::translate( "ActionParameterLimits", "Limit 2" ), "limit", false, true ), converter, line2 ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLimits::ActionParameterLimits( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterLimits::ReadLimit, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLimits::~ActionParameterLimits()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits::ReadLimit
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterLimits::ReadLimit( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterLimit( converter, xis ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_OrderContext& asn ) : asn_( &asn ) {}
        virtual void Visit( const ActionParameterLimit& param )
        {
            if( !asn_->m.limite_droitePresent )
            {
                asn_->m.limite_droitePresent = 1;
                param.CommitTo( asn_->limite_droite );
            }
            else if( !asn_->m.limite_gauchePresent )
            {
                asn_->m.limite_gauchePresent = 1;
                param.CommitTo( asn_->limite_gauche );
            }
        }

        ASN1T_OrderContext* asn_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLimits::CommitTo( ASN1T_OrderContext& asn ) const
{
    asn.m.limite_droitePresent = asn.m.limite_gauchePresent = 0;
    AsnSerializer serializer( asn );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_OrderContext& asn ) : asn_( &asn ) {}
        virtual void Visit( const ActionParameterLimit& param )
        {
            if( asn_->m.limite_droitePresent )
            {
                asn_->m.limite_droitePresent = 0;
                param.Clean( asn_->limite_droite );
            }
            else if( asn_->m.limite_gauchePresent )
            {
                asn_->m.limite_gauchePresent = 0;
                param.Clean( asn_->limite_gauche );
            }
        }

        ASN1T_OrderContext* asn_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLimits::Clean( ASN1T_OrderContext& asn ) const
{
    AsnCleaner cleaner( asn );
    Accept( cleaner );
}
