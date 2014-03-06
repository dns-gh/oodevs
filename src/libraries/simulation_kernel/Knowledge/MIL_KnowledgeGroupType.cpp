// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_KnowledgeGroupType.h"
#include "MIL_KnowledgeGroup.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"

#include <boost/ptr_container/ptr_map.hpp>

namespace
{
    boost::ptr_map< std::string, MIL_KnowledgeGroupType > knowledgeGroupTypes;
    unsigned int nNextID;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::InitializeWithTime
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::InitializeWithTime( xml::xistream& xis, double timeFactor )
{
    MT_LOG_INFO_MSG( "Initializing knowledge groups types" );
    xis >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", [&]( xml::xistream& xis ){
                const std::string name = xis.attribute< std::string >( "name" );
                if( knowledgeGroupTypes.count( name ) )
                    throw MASA_EXCEPTION( xis.context() + "Type already defined" );
                auto next = std::auto_ptr< MIL_KnowledgeGroupType >( new MIL_KnowledgeGroupType( name, xis, timeFactor ) );
                knowledgeGroupTypes.insert( name, next );
            })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Initialize
// Created: HBD 2009-12-28
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::Initialize( xml::xistream& xis )
{
    InitializeWithTime( xis, MIL_Tools::ConvertSecondsToSim( 1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::Terminate()
{
    knowledgeGroupTypes.clear();
}

namespace
{
    double ReadTime( xml::xisubstream xis, const std::string& start, const std::string& name,
                     bool required, bool positive )
    {
        if( !start.empty() )
            xis >> xml::start( start );
        double value = 0;
        const bool found = tools::ReadTimeAttribute( xis, name, value );
        if( found || required )
            if( value < 0 || positive && !value )
                throw MASA_EXCEPTION( xis.context() + ( start.empty() ? "" : name + ": " ) + name + " <= 0" );
        return value;
    }

    double ReadMaxDistance( xml::xisubstream xis )
    {
        xis >> xml::start( "unit-knowledge" );
        unsigned int nTmp = std::numeric_limits< unsigned int >::max();
        xis >> xml::optional >> xml::attribute( "max-unit-to-knowledge-distance", nTmp );
        if( nTmp <= 0 )
            throw MASA_EXCEPTION( xis.context() + "unit-knowledge: max-unit-to-knowledge-distance <= 0" );
        return MIL_Tools::ConvertMeterToSim( nTmp );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_KnowledgeGroupType::MIL_KnowledgeGroupType( const std::string& strName, xml::xistream& xis, double timeFactor )
    : strName_                                      ( strName )
    , nID_                                          ( nNextID++ )
    , rKnowledgeAgentMaxLifeTime_                   ( ReadTime( xis, "unit-knowledge", "max-lifetime", true, false ) * timeFactor )
    , rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_( ReadMaxDistance( xis ) )
    , rKnowledgeAgentExtrapolationTime_             ( std::max( 1., ReadTime( xis, "unit-knowledge", "interpolation-time", false, false ) * timeFactor ) )
    , rKnowledgePopulationMaxLifeTime_              ( ReadTime( xis, "population-knowledge", "max-lifetime", true, false ) * timeFactor )
    , rCommunicationDelay_                          ( ReadTime( xis, "", "communication-delay", true, true ) * timeFactor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_KnowledgeGroupType::~MIL_KnowledgeGroupType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Find
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindType( const std::string& strName )
{
    auto it = knowledgeGroupTypes.find( strName );
    return it == knowledgeGroupTypes.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::FindType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindType( unsigned int nID )
{
    for( auto it = knowledgeGroupTypes.begin(); it != knowledgeGroupTypes.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::FindTypeOrAny
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindTypeOrAny( const std::string& strName )
{
    const MIL_KnowledgeGroupType* type = FindType( strName );
    if( !type && !::knowledgeGroupTypes.empty() )
        type = knowledgeGroupTypes.begin()->second;
    return type;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetID
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
unsigned int MIL_KnowledgeGroupType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgePopulationMaxLifeTime
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
double MIL_KnowledgeGroupType::GetKnowledgePopulationMaxLifeTime() const
{
    return rKnowledgePopulationMaxLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentMaxLifeTime
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
double MIL_KnowledgeGroupType::GetKnowledgeAgentMaxLifeTime() const
{
    return rKnowledgeAgentMaxLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
double MIL_KnowledgeGroupType::GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() const
{
    return rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentExtrapolationTime
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
double MIL_KnowledgeGroupType::GetKnowledgeAgentExtrapolationTime() const
{
    return rKnowledgeAgentExtrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeCommunicationDelay
// Created: SLG 2009-11-29
// LTO
// -----------------------------------------------------------------------------
double MIL_KnowledgeGroupType::GetKnowledgeCommunicationDelay() const
{
    return rCommunicationDelay_;
}

// -----------------------------------------------------------------------------
// Name: mil_knowledgegrouptype::GetName
// Created: JVT 2005-03-01
// -----------------------------------------------------------------------------
const std::string& MIL_KnowledgeGroupType::GetName() const
{
    return strName_;
}
