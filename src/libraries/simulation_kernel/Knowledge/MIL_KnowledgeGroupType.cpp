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
#include "xeumeuleu/xml.h"

using namespace xml;

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

    xis >> start( "knowledge-groups" )
            >> list( "knowledge-group", loader, &LoadingWrapper::ReadKnowledgeGroup )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::ReadKnowledgeGroup
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::ReadKnowledgeGroup( xml::xistream& xis )
{
    std::string strName;
    std::string strType;

    xis >> attribute( "name", strName );

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

// =============================================================================
// INIT
// =============================================================================

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
{
    // Connaissances agent
    xis >> start( "unit-knowledge" );

    tools::ReadTimeAttribute( xis, "max-lifetime", rKnowledgeAgentMaxLifeTime_ );
    if( rKnowledgeAgentMaxLifeTime_ <= 0 )
        xis.error( "unit-knowledge: max-lifetime <= 0" );
    rKnowledgeAgentMaxLifeTime_ = MIL_Tools::ConvertSecondsToSim( rKnowledgeAgentMaxLifeTime_ );

    uint nTmp = std::numeric_limits< unsigned int >::max();
    xis >> optional() >> attribute( "max-unit-to-knowledge-distance", nTmp );
    if( nTmp <= 0 )
        xis.error( "unit-knowledge: max-unit-to-knowledge-distance <= 0" );
    rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_ = MIL_Tools::ConvertMeterToSim( nTmp );

    if( tools::ReadTimeAttribute(xis, "interpolation-time", rKnowledgeAgentExtrapolationTime_ ) )
        if( rKnowledgeAgentExtrapolationTime_ <= 0 )
            xis.error( "unit-knowledge: interpolation-time <= 0" );
    rKnowledgeAgentExtrapolationTime_ = std::max( 1., MIL_Tools::ConvertSecondsToSim( rKnowledgeAgentExtrapolationTime_ ) );
    // JVT : 1 car lorsque l'on perd de vue une unité, on veux au moins que l'emplacement de la connaissance soit celle au pas de temps suivant le non vu

    xis >> end();

    // Connaissances population
    xis >> start( "population-knowledge" );

    tools::ReadTimeAttribute( xis, "max-lifetime", rKnowledgePopulationMaxLifeTime_ );
    if( rKnowledgePopulationMaxLifeTime_ <= 0 )
        xis.error( "population-knowledge: max-lifetime <= 0" );

    rKnowledgePopulationMaxLifeTime_ = MIL_Tools::ConvertSecondsToSim( rKnowledgePopulationMaxLifeTime_ );

    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_KnowledgeGroupType::~MIL_KnowledgeGroupType()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::InstanciateKnowledgeGroup
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& MIL_KnowledgeGroupType::InstanciateKnowledgeGroup( uint nID, MIL_Army& army ) const
{
    return *new MIL_KnowledgeGroup( *this, nID, army );
}
