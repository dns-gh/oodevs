// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/MIL_KnowledgeGroupType.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 19:07 $
// $Revision: 5 $
// $Workfile: MIL_KnowledgeGroupType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_KnowledgeGroupType.h"
#include "MIL_KnowledgeGroup.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>

MIL_KnowledgeGroupType::T_KnowledgeGroupTypeMap MIL_KnowledgeGroupType::knowledgeGroupTypes_;
uint                                            MIL_KnowledgeGroupType::nNextID_ = 0;

struct MIL_KnowledgeGroupType::LoadingWrapper
{
    void ReadKnowledgeGroup( xml::xistream& xis )
    {
        MIL_KnowledgeGroupType::ReadKnowledgeGroup( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing knowledge groups types" );
    LoadingWrapper loader;

    xis >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", loader, &LoadingWrapper::ReadKnowledgeGroup )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::ReadKnowledgeGroup
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::ReadKnowledgeGroup( xml::xistream& xis )
{
    std::string strName;
    std::string strType;
    

    xis >> xml::attribute( "name", strName );
    

    const MIL_KnowledgeGroupType*& pType = knowledgeGroupTypes_[ strName ];
    if( pType )
        xis.error( "Type already defined" );
    pType = new MIL_KnowledgeGroupType( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::Terminate()
{
    for( CIT_KnowledgeGroupTypeMap it = knowledgeGroupTypes_.begin(); it != knowledgeGroupTypes_.end(); ++it )
        delete it->second;
    knowledgeGroupTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_KnowledgeGroupType::MIL_KnowledgeGroupType( const std::string& strName, xml::xistream& xis )
    : strName_                                      ( strName )
    , nID_                                          ( nNextID_++ )
    , rKnowledgeAgentMaxLifeTime_                   ( 0. )
    , rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_( 0. )
    , rKnowledgeAgentExtrapolationTime_             ( 0. )
    , rKnowledgePopulationMaxLifeTime_              ( 0. )
    , rCommunicationDelay_                          ( 0. )
{
    tools::ReadTimeAttribute( xis, "communication-delay", rCommunicationDelay_ );
    if( rCommunicationDelay_ < 0 )
        xis.error( "unit-knowledge: max-lifetime <= 0" );
    rCommunicationDelay_ = MIL_Tools::ConvertSecondsToSim( rCommunicationDelay_ );

    // Connaissances agent
    xis >> xml::start( "unit-knowledge" );

    tools::ReadTimeAttribute( xis, "max-lifetime", rKnowledgeAgentMaxLifeTime_ );
    if( rKnowledgeAgentMaxLifeTime_ <= 0 )
        xis.error( "unit-knowledge: max-lifetime <= 0" );
    rKnowledgeAgentMaxLifeTime_ = MIL_Tools::ConvertSecondsToSim( rKnowledgeAgentMaxLifeTime_ );

    uint nTmp = std::numeric_limits< unsigned int >::max();
    xis >> xml::optional() >> xml::attribute( "max-unit-to-knowledge-distance", nTmp );
    if( nTmp <= 0 )
        xis.error( "unit-knowledge: max-unit-to-knowledge-distance <= 0" );
    rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_ = MIL_Tools::ConvertMeterToSim( nTmp );

    if( tools::ReadTimeAttribute(xis, "interpolation-time", rKnowledgeAgentExtrapolationTime_ ) )
        if( rKnowledgeAgentExtrapolationTime_ <= 0 )
            xis.error( "unit-knowledge: interpolation-time <= 0" );
    rKnowledgeAgentExtrapolationTime_ = std::max( 1., MIL_Tools::ConvertSecondsToSim( rKnowledgeAgentExtrapolationTime_ ) );
    // JVT : 1 car lorsque l'on perd de vue une unité, on veux au moins que l'emplacement de la connaissance soit celle au pas de temps suivant le non vu

    xis >> xml::end();

    // Connaissances population
    xis >> xml::start( "population-knowledge" );

    tools::ReadTimeAttribute( xis, "max-lifetime", rKnowledgePopulationMaxLifeTime_ );
    if( rKnowledgePopulationMaxLifeTime_ <= 0 )
        xis.error( "population-knowledge: max-lifetime <= 0" );

    rKnowledgePopulationMaxLifeTime_ = MIL_Tools::ConvertSecondsToSim( rKnowledgePopulationMaxLifeTime_ );

    xis >> xml::end();
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
// Name: MIL_KnowledgeGroupType::InstanciateKnowledgeGroup
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& MIL_KnowledgeGroupType::InstanciateKnowledgeGroup( uint nID, MIL_Army& army ) const
{
    return *new MIL_KnowledgeGroup( *this, nID, army );
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
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindType( uint nID )
{
    CIT_KnowledgeGroupTypeMap it = std::find_if( knowledgeGroupTypes_.begin(), knowledgeGroupTypes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &MIL_KnowledgeGroupType::GetID ), std::select2nd< T_KnowledgeGroupTypeMap::value_type >() ) ) );
    return it == knowledgeGroupTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetID
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
uint MIL_KnowledgeGroupType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgePopulationMaxLifeTime
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroupType::GetKnowledgePopulationMaxLifeTime() const
{
    return rKnowledgePopulationMaxLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentMaxLifeTime
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroupType::GetKnowledgeAgentMaxLifeTime() const
{
    return rKnowledgeAgentMaxLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroupType::GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() const
{
    return rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentExtrapolationTime
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroupType::GetKnowledgeAgentExtrapolationTime() const
{
    return rKnowledgeAgentExtrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeCommunicationDelay
// Created: SLG 2009-11-29
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroupType::GetKnowledgeCommunicationDelay() const
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
