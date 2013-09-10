// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_KnowledgeGroupType.h"
#include "MIL_KnowledgeGroup.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include <xeumeuleu/xml.hpp>

MIL_KnowledgeGroupType::T_KnowledgeGroupTypeMap MIL_KnowledgeGroupType::knowledgeGroupTypes_;
unsigned int MIL_KnowledgeGroupType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::InitializeWithTime
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::InitializeWithTime( xml::xistream& xis, double timeFactor )
{
    MT_LOG_INFO_MSG( "Initializing knowledge groups types" );
    xis >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", boost::bind( &MIL_KnowledgeGroupType::ReadKnowledgeGroup, _1, timeFactor ) )
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
// Name: MIL_KnowledgeGroupType::ReadKnowledgeGroup
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::ReadKnowledgeGroup( xml::xistream& xis, double timeFactor )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );
    const MIL_KnowledgeGroupType*& pType = knowledgeGroupTypes_[ strName ];
    if( pType )
        throw MASA_EXCEPTION( xis.context() + "Type already defined" );
    pType = new MIL_KnowledgeGroupType( strName, xis, timeFactor );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::Terminate()
{
    for( auto it = knowledgeGroupTypes_.begin(); it != knowledgeGroupTypes_.end(); ++it )
        delete it->second;
    knowledgeGroupTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_KnowledgeGroupType::MIL_KnowledgeGroupType( const std::string& strName, xml::xistream& xis, double timeFactor )
    : strName_                                      ( strName )
    , nID_                                          ( nNextID_++ )
    , rKnowledgeAgentMaxLifeTime_                   ( 0. )
    , rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_( 0. )
    , rKnowledgeAgentExtrapolationTime_             ( 0. )
    , rKnowledgePopulationMaxLifeTime_              ( 0. )
    , rCommunicationDelay_                          ( 0. ) // LTO
{
    tools::ReadTimeAttribute( xis, "communication-delay", rCommunicationDelay_ ); // LTO
    if( rCommunicationDelay_ < 0 ) // LTO
        throw MASA_EXCEPTION( xis.context() + "unit-knowledge: max-lifetime <= 0" ); // LTO
    rCommunicationDelay_ *= timeFactor; // LTO
    // Connaissances agent
    xis >> xml::start( "unit-knowledge" );
    tools::ReadTimeAttribute( xis, "max-lifetime", rKnowledgeAgentMaxLifeTime_ );
    if( rKnowledgeAgentMaxLifeTime_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "unit-knowledge: max-lifetime <= 0" );
    rKnowledgeAgentMaxLifeTime_ = timeFactor * rKnowledgeAgentMaxLifeTime_; // LTO
    unsigned int nTmp = std::numeric_limits< unsigned int >::max();
    xis >> xml::optional >> xml::attribute( "max-unit-to-knowledge-distance", nTmp );
    if( nTmp <= 0 )
        throw MASA_EXCEPTION( xis.context() + "unit-knowledge: max-unit-to-knowledge-distance <= 0" );
    rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_ = MIL_Tools::ConvertMeterToSim( nTmp );
    if( tools::ReadTimeAttribute(xis, "interpolation-time", rKnowledgeAgentExtrapolationTime_ ) )
        if( rKnowledgeAgentExtrapolationTime_ <= 0 )
            throw MASA_EXCEPTION( xis.context() + "unit-knowledge: interpolation-time <= 0" );
    rKnowledgeAgentExtrapolationTime_ = std::max( 1., timeFactor * rKnowledgeAgentExtrapolationTime_ );
    // JVT : 1 car lorsque l'on perd de vue une unit�, on veux au moins que l'emplacement de la connaissance soit celle au pas de temps suivant le non vu
    xis >> xml::end;
    // Connaissances population
    xis >> xml::start( "population-knowledge" );
    tools::ReadTimeAttribute( xis, "max-lifetime", rKnowledgePopulationMaxLifeTime_ );
    if( rKnowledgePopulationMaxLifeTime_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "population-knowledge: max-lifetime <= 0" );
    rKnowledgePopulationMaxLifeTime_ = timeFactor * rKnowledgePopulationMaxLifeTime_;
    xis >> xml::end;
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
    CIT_KnowledgeGroupTypeMap it = knowledgeGroupTypes_.find( strName );
    return it == knowledgeGroupTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::FindType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindType( unsigned int nID )
{
    for( auto it = knowledgeGroupTypes_.begin(); it != knowledgeGroupTypes_.end(); ++it )
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
    if( !type && !knowledgeGroupTypes_.empty() )
        type = knowledgeGroupTypes_.begin()->second;
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
