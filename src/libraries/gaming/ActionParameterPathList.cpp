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
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_PathList& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterPath( OrderParameter( tools::translate( "ActionParameter", "Route %1" ).arg( i ), "path", false ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
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
void ActionParameterPathList::ReadPath( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterPath( converter, xis ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_PathList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterPath& param )
        {
            if( current_ < asn_->n )
                param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_PathList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPathList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_pathList;
    ASN1T_PathList*& list = asn.value.u.pathList = new ASN1T_PathList();
    asn.null_value = ( list->n = Count() ) ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Path[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_PathList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterPath& param )
        {
            if( current_ < asn_->n )
                param.Clean( asn_->elem[current_++] );
        }

        ASN1T_PathList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPathList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.pathList )
    {
        AsnCleaner cleaner( *asn.value.u.pathList );
        Accept( cleaner );
    }
    delete[] asn.value.u.pathList;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterPathList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}
