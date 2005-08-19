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

#include "MIL_pch.h"

#include "MIL_KnowledgeGroupType.h"
#include "MIL_KnowledgeGroup.h"
#include "Tools/MIL_Tools.h"

MIL_KnowledgeGroupType::T_KnowledgeGroupTypeMap MIL_KnowledgeGroupType::knowledgeGroupTypes_;
uint                                            MIL_KnowledgeGroupType::nNextID_             = 0;

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroupType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing knowledge groups types" );

    archive.BeginList( "GroupesConnaissance" );
    while( archive.NextListElement() )
    {
        archive.Section( "GroupeConnaissance" );
    
        std::string strName;
        std::string strType;

        archive.ReadAttribute( "nom", strName );

        const MIL_KnowledgeGroupType*& pType = knowledgeGroupTypes_[ strName ];
        if( pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Type already defined", archive.GetContext() );
        pType = new MIL_KnowledgeGroupType( strName, archive );

        archive.EndSection(); // GroupeConnaissance
    }
    archive.EndList(); // GroupesConnaissance
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
MIL_KnowledgeGroupType::MIL_KnowledgeGroupType( const std::string& strName, MIL_InputArchive& archive )
    : strName_                                 ( strName )
    , nID_                                     ( nNextID_++ )
    , rKnowledgeMaxLifeTime_                   ( 0. )
    , rKnowledgeMaxDistBtwKnowledgeAndRealUnit_( 0. )
    , rKnowledgeExtrapolationTime_             ( 0. )
{
    // Connaissance
    archive.Section( "Connaissance" );

    archive.ReadTimeField( "DureeDeVieMax", rKnowledgeMaxLifeTime_, CheckValueGreater( 0. ) );
    rKnowledgeMaxLifeTime_ = MIL_Tools::ConvertSecondsToSim( rKnowledgeMaxLifeTime_ );

    uint nTmp;
    archive.ReadField( "DistanceMaxEntreUniteReelleEtUniteConnue", nTmp, CheckValueGreater( (uint)0 ) );
    rKnowledgeMaxDistBtwKnowledgeAndRealUnit_ = MIL_Tools::ConvertMeterToSim( nTmp );

    archive.ReadTimeField( "TempsInterpolation", rKnowledgeExtrapolationTime_, CheckValueGreater( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );
    rKnowledgeExtrapolationTime_ = std::max( 1., MIL_Tools::ConvertSecondsToSim( rKnowledgeExtrapolationTime_ ) );
    // JVT : 1 car lorsque l'on perd de vue une unité, on veux au moins que l'emplacement de la connaissance soit celle au pas de temps suivant le non vu

    archive.EndSection(); // Connaissance
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_KnowledgeGroupType::~MIL_KnowledgeGroupType()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::InstanciateKnowledgeGroup
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& MIL_KnowledgeGroupType::InstanciateKnowledgeGroup( uint nID, MIL_Army& army, MIL_InputArchive& archive ) const
{
    return *new MIL_KnowledgeGroup( *this, nID, army, archive );
}
