// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PathList.h"
#include "Path.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
PathList::PathList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
PathList::PathList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_PathList& asn )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Path( OrderParameter( tools::translate( "Parameter", "Route %1" ).arg( i ).ascii(), "path", false ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: PathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
PathList::PathList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &PathList::ReadPath, converter );
}

// -----------------------------------------------------------------------------
// Name: PathList destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
PathList::~PathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathList::ReadPath
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void PathList::ReadPath( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new Path( converter, xis ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_PathList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Path& param )
        {
            if( current_ < asn_->n )
                param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_PathList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: PathList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PathList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_pathList;
    ASN1T_PathList*& list = asn.value.u.pathList = new ASN1T_PathList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_Path[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

namespace
{
    struct AsnCleaner : public ParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_PathList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Path& param )
        {
            if( current_ < asn_->n )
                param.Clean( asn_->elem[current_++] );
        }

        ASN1T_PathList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: PathList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PathList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.pathList )
    {
        AsnCleaner cleaner( *asn.value.u.pathList );
        Accept( cleaner );
        delete[] asn.value.u.pathList;
    }
}

// -----------------------------------------------------------------------------
// Name: PathList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void PathList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: PathList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool PathList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
