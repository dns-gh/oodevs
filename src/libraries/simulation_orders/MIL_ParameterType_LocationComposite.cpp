// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_LocationComposite.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationComposite constructor
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationComposite::MIL_ParameterType_LocationComposite( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationComposite destructor
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationComposite::~MIL_ParameterType_LocationComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationComposite::Copy
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
bool MIL_ParameterType_LocationComposite::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    to.set_null_value( !from.ToPoint( *to.mutable_value()->mutable_point() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToPath( (Common::MsgPath&)*to.mutable_value()->mutable_path() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToPolygon( *to.mutable_value()->mutable_polygon() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToAutomat( *to.mutable_value()->mutable_automat() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToAgent( *to.mutable_value()->mutable_unit() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToAgentKnowledge( *to.mutable_value()->mutable_unitknowledge() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToObjectKnowledge( *to.mutable_value()->mutable_objectknowledge() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToPopulationKnowledge( *to.mutable_value()->mutable_crowdknowledge() ) );
    if( to.null_value() )
        to.clear_value();
    else
        return true;
    to.set_null_value( !from.ToUrbanBlock( *to.mutable_value()->mutable_urbanblock() ) );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional;
}
