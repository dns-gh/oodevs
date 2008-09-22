// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "LimaList.h"
#include "Lima.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
LimaList::LimaList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
LimaList::LimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& asn )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Lima( OrderParameter( tools::translate( "Parameter", "Phase line %1" ).arg( i ).ascii(), "lima", true ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: LimaList constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
LimaList::LimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &LimaList::ReadLima, converter );
}

// -----------------------------------------------------------------------------
// Name: LimaList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
LimaList::~LimaList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimaList::ReadLima
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void LimaList::ReadLima( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new Lima( converter, xis ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_LimasOrder& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Lima& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }
        ASN1T_LimasOrder* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: LimaList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LimaList::CommitTo( ASN1T_MissionParameter& asn ) const
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
    struct AsnCleaner : public ParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_LimasOrder& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Lima& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
        ASN1T_LimasOrder* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: LimaList::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LimaList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.limasOrder )
    {
        AsnCleaner cleaner( *asn.value.u.limasOrder );
        Accept( cleaner );
        delete[] asn.value.u.limasOrder;
    }
}

// -----------------------------------------------------------------------------
// Name: LimaList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void LimaList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: LimaList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool LimaList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
