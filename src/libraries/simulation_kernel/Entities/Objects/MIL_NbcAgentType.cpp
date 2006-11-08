//*****************************************************************************
//
// $Created: JVT 03-01-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NbcAgentType.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:31 $
// $Revision: 10 $
// $Workfile: MIL_NbcAgentType.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_NbcAgentType.h"

#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Tools/MIL_Tools.h"

MIL_NbcAgentType::T_NbcAgentTypeMap MIL_NbcAgentType::nbcAgentTypes_;
MT_Float                            MIL_NbcAgentType::rCoefMaxSpeedModificator_      = 1.;
MT_Float                            MIL_NbcAgentType::rCoefReloadingTimeModificator_ = 1.;
MT_Float                            MIL_NbcAgentType::rMinPropagationSpeed_          = 0.;
MT_Random                           MIL_NbcAgentType::randomGenerator_;

// =============================================================================
// FACTORY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::Initialize
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing nbc types" );

    archive.Section  ( "NBC" );
    archive.Section  ( "ImpactTenueNBC" );
    archive.ReadField( "CoefModificationVitesseMax", rCoefMaxSpeedModificator_, CheckValueGreaterOrEqual( 0. ) );
    
    archive.ReadField( "CoefModificationTempsRechargement", rCoefReloadingTimeModificator_, CheckValueGreater( 0. ) );
    archive.EndSection(); // ImpactTenueNBC

    archive.ReadField( "VitesseVentLimiteAvantPropagationDirectionnelle", rMinPropagationSpeed_, CheckValueGreaterOrEqual( 0. ) );
    rMinPropagationSpeed_ = MIL_Tools::ConvertSpeedMosToSim( rMinPropagationSpeed_ );
   
    std::set< uint > ids_;
    archive.BeginList( "AgentsNBC" );
    while ( archive.NextListElement() )
    {
        std::string strName;
        archive.Section( "AgentNBC" );
        archive.ReadAttribute( "nom", strName );

        const MIL_NbcAgentType*& pNbcAgentType = nbcAgentTypes_[ strName ];
        if( pNbcAgentType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown NBC agent type", archive.GetContext() );
        pNbcAgentType = new MIL_NbcAgentType( strName, archive );

        if( !ids_.insert( pNbcAgentType->GetID() ).second )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "NBC agent type ID already used", archive.GetContext() );

        archive.EndSection(); // AgentNBC
    }
    archive.EndList(); // AgentsNBC
    archive.EndSection(); // NBC
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::Terminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::Terminate()
{
    for( CIT_NbcAgentTypeMap it = nbcAgentTypes_.begin(); it != nbcAgentTypes_.end(); ++it )
        delete it->second;
    nbcAgentTypes_.clear();
}

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType constructor
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
MIL_NbcAgentType::MIL_NbcAgentType( const std::string& strName, MIL_InputArchive& archive )
    : strName_             ( strName )
    , nID_                 ( 0 )
    , liquidPoisonous_     ( PHY_HumanWound::GetHumanWounds().size(), 0. )
    , bLiquidContaminating_( false )
    , bLiquidPoisonous_    ( false )
    , bCanBeVaporized_     ( false )
    , gasPoisonous_        ( PHY_HumanWound::GetHumanWounds().size(), 0. )
    , bGasContaminating_   ( false )
    , bGasPoisonous_       ( false )
    , nGasLifeTime_        ( 0 )
    , rGasPropagationAngle_( 0. )
{
    archive.ReadField( "MosID", nID_ );

    archive.Section( "Liquide" );
    if( ReadPoisonousData( archive, liquidPoisonous_ ) )
        bLiquidPoisonous_ = true;
    if( archive.Section( "Contamination", MIL_InputArchive::eNothing ) )
    {
        bLiquidContaminating_ = true;
        archive.EndSection(); // Contamination
    }
    archive.EndSection(); // Liquide

    if( archive.Section( "Gaz", MIL_InputArchive::eNothing ) )
    {
        bCanBeVaporized_ = true;
        if( ReadPoisonousData( archive, gasPoisonous_ ) )
            bGasPoisonous_ = true;
        if( archive.Section( "Contamination", MIL_InputArchive::eNothing ) )
        {
            bLiquidContaminating_ = true;
            archive.EndSection(); // Contamination
        }
        archive.ReadTimeField( "DureeDeVie", nGasLifeTime_, CheckValueGreater( 0 ) );
        nGasLifeTime_ = (uint)MIL_Tools::ConvertSecondsToSim( nGasLifeTime_ );

        archive.Section( "AngleDePropagation" );
        archive.Read( rGasPropagationAngle_, CheckValueGreater( 0. ) );
        std::string strUnit;
        archive.ReadAttribute( "unite", strUnit );
        if ( sCaseInsensitiveEqual()( strUnit, "degre" ) )
            rGasPropagationAngle_ *= ( MT_PI / 180. );
        else if ( !sCaseInsensitiveEqual()( strUnit, "radian" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown angle unit", archive.GetContext() );
        archive.EndSection(); // AngleDePropagation 

        archive.EndSection(); // Gaz
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType destructor
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
MIL_NbcAgentType::~MIL_NbcAgentType()
{
     
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ReadPoisonousData
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::ReadPoisonousData( MIL_InputArchive& archive, T_HumanPoisonousVector& data )
{
    if( !archive.Section( "Intoxication", MIL_InputArchive::eNothing ) )
        return false;

    float rTotalValue = 0.;
    const PHY_HumanWound::T_HumanWoundMap& humanWounds = PHY_HumanWound::GetHumanWounds();
    for( PHY_HumanWound::CIT_HumanWoundMap itWound = humanWounds.begin(); itWound != humanWounds.end(); ++itWound )
    {
        const PHY_HumanWound& humanWound = *itWound->second;

        assert( data.size() > humanWound.GetID() );
        std::stringstream strTagName;
        strTagName << "Humains_" << humanWound.GetName();

        float rValue;
        archive.ReadField( strTagName.str(), rValue, CheckValueBound( 0., 1. ) );
        data[ humanWound.GetID() ] = rValue;
        rTotalValue += rValue;
    }
    if ( rTotalValue != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Sum of poisonous percentage is out of bound (%.2f) for NBC Agent type '%s'", rTotalValue, strName_.c_str() ), archive.GetContext() );
    archive.EndSection(); // Intoxication
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetRandomWound
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_NbcAgentType::GetRandomWound( const T_HumanPoisonousVector& data ) const
{
    const MT_Float rRand = randomGenerator_.rand_ii( 0., 1. );

    MT_Float rSumCoefs = 0.;
    const PHY_HumanWound::T_HumanWoundMap& humanWounds = PHY_HumanWound::GetHumanWounds();
    for( PHY_HumanWound::CIT_HumanWoundMap itWound = humanWounds.begin(); itWound != humanWounds.end(); ++itWound )
    {
        const PHY_HumanWound& wound = *itWound->second;
        rSumCoefs += data[ wound.GetID() ];
        if( rSumCoefs >= rRand )
            return wound;
    }
    assert( false );
    return PHY_HumanWound::killed_;
}

