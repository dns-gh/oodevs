// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ListParameter.h"
#include "MIL_MissionParameterFactory.h"
#include "knowledge/DEC_KnowledgeResolver_ABC.h"
#include "knowledge/DEC_Knowledge_Urban.h"
#include "simulation_orders/MIL_ParameterType_LocationCompositeList.h"
#include "protocol/protocol.h"
#include "urban/TerrainObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter constructor
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
MIL_ListParameter::MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >& list )
    : resolver_ (resolver )
{
    for( ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >::const_iterator it = list.begin(); it != list.end(); ++it )
    {
        list_.push_back( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter constructor
// Created: MGD 2010-10-19
// -----------------------------------------------------------------------------
MIL_ListParameter::MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const std::vector< boost::shared_ptr< DEC_Knowledge_Urban > >& urbanBlockList )
    : resolver_ (resolver )
{
    for( std::vector< boost::shared_ptr< DEC_Knowledge_Urban > >::const_iterator it = urbanBlockList.begin(); it != urbanBlockList.end(); it++ )
    {
        Common::MsgMissionParameter_Value block;
        block.mutable_urbanblock()->set_id( (*it)->GetTerrainObjectKnown().GetId() );
        list_.push_back( block );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter destructor
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
MIL_ListParameter::~MIL_ListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::IsOfType
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
bool MIL_ListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_LocationCompositeList*>( &type ) != 0 );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToList
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToList( std::vector< Common::MsgMissionParameter_Value >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
        result.push_back( *it );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToList
// Created: LDC 2010-09-22
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr<MIL_MissionParameter_ABC> param = MIL_MissionParameterFactory::Create( *it, resolver_ );
        result.push_back( param );
    }
    return true;
}
