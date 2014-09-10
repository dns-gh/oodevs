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
LimaList::LimaList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::PhaseLinesOrder& message,
                    const kernel::Entity_ABC* owner, kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter,
                    kernel::Controller& controller, const kernel::EntityResolver_ABC& entities )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Lima( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).toAscii().constData(), "phaseline", true ), converter, message.elem( i ), owner, objectKnowledgeConverter, controller, entities ) );
}

// -----------------------------------------------------------------------------
// Name: LimaList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
LimaList::~LimaList()
{
    // NOTHING
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( sword::PhaseLinesOrder& message )
            : message_( &message )
        {}
        virtual void Visit( const Lima& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        sword::PhaseLinesOrder* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: LimaList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LimaList::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        AsnSerializer serializer( *message.add_value()->mutable_phaseline() );
        Accept( serializer );
    }
}
// -----------------------------------------------------------------------------
// Name: LimaList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LimaList::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
    {
        AsnSerializer serializer( *message.mutable_phaseline() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: LimaList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void LimaList::DisplayTooltip( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: LimaList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool LimaList::IsSet() const
{
    return( Parameter< QString >::IsSet() && IsInRange() );
    // $$$$ SBO 2008-03-19: each element must be set as well...
}

// -----------------------------------------------------------------------------
// Name: LimaList::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string LimaList::SerializeType() const
{
    return "phaseline";
}
