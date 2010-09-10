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
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

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
PathList::PathList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgPathList& message )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Path( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "path", false ), converter, message.elem(i).location() ) );
}

// -----------------------------------------------------------------------------
// Name: PathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
PathList::PathList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &PathList::ReadPath, converter );
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
    struct MessageSerializer : public ParameterVisitor_ABC
    {
        explicit MessageSerializer( Common::MsgPathList& message ) : message_( &message ) {}
        virtual void Visit( const Path& param )
        {
            param.CommitTo( *message_->add_elem()->mutable_location() );
        }
        Common::MsgPathList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: PathList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PathList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        MessageSerializer serializer( *message.mutable_value()->mutable_pathlist() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: PathList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PathList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_pathlist() )
        message.mutable_value()->clear_pathlist();
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
