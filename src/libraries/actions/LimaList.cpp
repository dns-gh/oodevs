// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "LimaList.h"
#include "Lima.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

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
LimaList::LimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgLimasOrder& message )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Lima( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "lima", true ), converter, message.elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: LimaList constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
LimaList::LimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &LimaList::ReadLima, converter );
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
        explicit AsnSerializer( Common::MsgLimasOrder& message ) : message_( &message ) {}
        virtual void Visit( const Lima& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgLimasOrder* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: LimaList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LimaList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        AsnSerializer serializer( *message.mutable_value()->mutable_limasorder() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: LimaList::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LimaList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_limasorder() )
        message.mutable_value()->clear_limasorder();
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
