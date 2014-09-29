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
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "protocol/Protocol.h"
#include "MIL.h"
#include <boost/optional.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ListParameter )

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_ListParameter::MIL_ListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter constructor
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
MIL_ListParameter::MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list )
{
    for( auto it = list.begin(); it != list.end(); ++it )
        list_.push_back( MIL_MissionParameterFactory::Create( *it, resolver, boost::none ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter constructor
// Created: MGD 2010-10-19
// -----------------------------------------------------------------------------
MIL_ListParameter::MIL_ListParameter( const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& paramList )
{
    for( auto it = paramList.begin(); it != paramList.end(); ++it )
        list_.push_back( *it );
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
        if( !(*it)->IsOfType( type ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToList
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& message ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
        if( !(*it)->ToElement( *message.Add() ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToElement( sword::MissionParameter_Value& message ) const
{
    return ToList( *message.mutable_list() );
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToList
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
        result.push_back( *it );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToNumericList
// Created: MMC 2011-03-23
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToNumericList( std::vector< float >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        float param = 0.0f;
        if( !(*it)->ToNumeric( param ) )
            return false;
        result.push_back( param );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToPathList
// Created: MGD 2010-11-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& result ) const
{
    // $$$$ MGD 2010-11-15 ALL ToList can be merge with a refactor in DEC_Decision
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
bool MIL_ListParameter::ToAgentList( std::vector< const DEC_Decision_ABC* >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        const DEC_Decision_ABC* param;
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
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
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< DEC_Gen_Object > param;
        if( !(*it)->ToGenObject( param ) )
            return false;
        result.push_back( param );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToDotationTypeList
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToDotationTypeList( std::vector< const PHY_DotationCategory* >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        const PHY_DotationCategory* param = 0;
        if( !(*it)->ToDotationType( param ) )
            return false;
        result.push_back( param );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToUrbanBlockList
// Created: ABR 2013-11-28
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToUrbanBlockList( std::vector< MIL_UrbanObject_ABC* >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        MIL_UrbanObject_ABC* param;
        if( !(*it)->ToUrbanBlock( param ) )
            return false;
        result.push_back( param );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToResourceNetworkNodeList
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToResourceNetworkNodeList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        boost::shared_ptr< DEC_ResourceNetwork > param;
        if( !(*it)->ToResourceNetworkNode( param ) )
            return false;
        result.push_back( param );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::ToResourceNetworkTypeList
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool MIL_ListParameter::ToResourceNetworkTypeList( std::vector< const PHY_ResourceNetworkType* >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        const PHY_ResourceNetworkType* param = 0;
        if( !(*it)->ToResourceNetworkType( param ) )
            return false;
        result.push_back( param );
    }
    return true;
}

bool MIL_ListParameter::ToEquipmentTypeList( std::vector< const PHY_ComposanteTypePion* >& result ) const
{
    for( auto it = list_.begin(); it != list_.end(); ++it )
    {
        const PHY_ComposanteTypePion* param = 0;
        if( !(*it)->ToEquipmentType( param ) )
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

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::load
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_ListParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> list_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ListParameter::save
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_ListParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << list_;
}
