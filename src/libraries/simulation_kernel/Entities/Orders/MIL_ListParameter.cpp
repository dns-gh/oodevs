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
        list_.push_back( MIL_MissionParameterFactory::Create( *it, resolver_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter constructor
// Created: MGD 2010-10-19
// -----------------------------------------------------------------------------
MIL_ListParameter::MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& paramList )
    : resolver_ (resolver )
{
    for( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >::const_iterator it = paramList.begin(); it != paramList.end(); it++ )
        list_.push_back( *it);
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
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_ListParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
        if( !(*it)->IsOfType( type ) )
            return false;
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToList
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToList( ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >& message ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
        if( !(*it)->ToElement( *message.Add() ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToElement( Common::MsgMissionParameter_Value& message ) const
{
    return ToList( *message.mutable_list() );
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToList
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
        result.push_back( *it );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToPathList 
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& result ) const
{
    // $$$$ MGD 2010-11-15 ALL ToList can be merge with a refactor in DEC_Decision
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        std::vector< boost::shared_ptr< MT_Vector2D > > param;
        if( !(*it)->ToPath( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}


// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToPointList
// Created: MGD 2010-11-09
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< MT_Vector2D > param;
        if( !(*it)->ToPoint( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToPolygonList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< TER_Localisation > param;
        if( !(*it)->ToPolygon( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToLocationList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< TER_Localisation > param;
        if( !(*it)->ToLocation( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToAutomatList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToAutomatList( std::vector< DEC_Decision_ABC* >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        DEC_Decision_ABC* param;
        if( !(*it)->ToAutomat( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToAgentList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToAgentList( std::vector< DEC_Decision_ABC* >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        DEC_Decision_ABC* param;
        if( !(*it)->ToAgent( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToAgentKnowledgeList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > param;
        if( !(*it)->ToAgentKnowledge( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToObjectKnowledgeList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Object >  param;
        if( !(*it)->ToObjectKnowledge( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToGenObjectList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& result ) const
{
    for( CIT_ParameterList it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< DEC_Gen_Object > param;
        if( !(*it)->ToGenObject( param ) )
            return false;
        result.push_back( param );
    }  
    return true;
}


// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::Append
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
void MIL_ListParameter::Append( boost::shared_ptr< MIL_MissionParameter_ABC > param )
{
    list_.push_back( param );
}
