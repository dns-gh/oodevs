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
#include "Entities/Populations/MIL_IntoxicationEffect.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <numeric>

MIL_NbcAgentType::T_NbcAgentTypeMap MIL_NbcAgentType::nbcAgentTypes_;
double MIL_NbcAgentType::rCoefMaxSpeedModificator_      = 1.;
double MIL_NbcAgentType::rCoefReloadingTimeModificator_ = 1.;
double MIL_NbcAgentType::rContaminationDistance_        = 100.;
double MIL_NbcAgentType::rContaminationQuantityGiven_   = 0.5;

struct MIL_NbcAgentType::LoadingWrapper
{
    void ReadAgent( xml::xistream& xis )
    {
        MIL_NbcAgentType::ReadAgent( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::Initialize
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing nbc types" );
    xis >> xml::start( "nbc" )
            >> xml::start( "nbc-suit" )
                >> xml::attribute( "max-speed-modifier", rCoefMaxSpeedModificator_ )
                >> xml::attribute( "reloading-time-modifier", rCoefReloadingTimeModificator_ )
            >> xml::end
            >> xml::start( "propagation" )
                >> xml::attribute( "contamination-distance", rContaminationDistance_ )
                >> xml::attribute( "contamination-quantity-given", rContaminationQuantityGiven_ )
            >> xml::end;

    if( rCoefMaxSpeedModificator_ < 0 )
        xis.error( "nbc-suit: max-speed-modifier < 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        xis.error( "nbc-suit: reloading-time-modifier <= 0" );

    if( rContaminationDistance_ < 0 )
        xis.error( "propagation: contamination-distance < 0" );

    LoadingWrapper loader;

    xis >> xml::start( "agents" )
            >> xml::list( "agent", loader, &LoadingWrapper::ReadAgent )
        >> xml::end
    >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ReadAgent
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::ReadAgent( xml::xistream& xis )
{
    std::set< unsigned int > ids_;
    std::string strName;

    xis >> xml::attribute( "name", strName );

    const MIL_NbcAgentType*& pNbcAgentType = nbcAgentTypes_[ strName ];
    if( pNbcAgentType )
        xis.error( "Unknown NBC agent type" );
    pNbcAgentType = new MIL_NbcAgentType( strName, xis );

    if( !ids_.insert( pNbcAgentType->GetID() ).second )
        xis.error( "NBC agent type ID already used" );
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

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType constructor
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
MIL_NbcAgentType::MIL_NbcAgentType( const std::string& strName, xml::xistream& xis )
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
    xis >> xml::attribute( "id", nID_ )
        >> xml::list( "effects", *this, &MIL_NbcAgentType::SortEffectType );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::SortEffectType
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::SortEffectType( xml::xistream& xis )
{
    std::string effectType;
    xis >> xml::attribute( "type", effectType );
    if( effectType == "liquid" )
        ReadLiquid( xis );
    else if( effectType == "gaseous" )
        ReadGaz( xis );
    else
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "effects: unknow type" );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ReadLiquid
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::ReadLiquid( xml::xistream& xis )
{
    if( ReadPoisonousData( xis, liquidPoisonous_ ) )
        bLiquidPoisonous_ = true;

    xis >> xml::optional >> xml::attribute( "contamination", bLiquidContaminating_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ReadGaz
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::ReadGaz( xml::xistream& xis )
{
    bCanBeVaporized_ = true;
    if( ReadPoisonousData( xis, gasPoisonous_ ) )
        bGasPoisonous_ = true;
    xis >> xml::optional >> xml::attribute( "contamination", bGasContaminating_ );

    tools::ReadTimeAttribute( xis, "life-time", nGasLifeTime_ );
    if( nGasLifeTime_ <= 0 )
        xis.error( "effects: life-time <= 0" );
    nGasLifeTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( nGasLifeTime_ );

    xis >> xml::attribute( "propagation", rGasPropagationAngle_ );
    if( rGasPropagationAngle_ <= 0 )
        xis.error( "effects: propagation <= 0" );
    rGasPropagationAngle_ *= ( MT_PI / 180. );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType destructor
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
MIL_NbcAgentType::~MIL_NbcAgentType()
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ReadPoisonousData
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::ReadPoisonousData( xml::xistream& xis, T_HumanPoisonousVector& data )
{
    bool intoxication = false;
    xis >> xml::optional >> xml::attribute( "affliction", intoxication );
    if( !intoxication )
        return false;
    xis >> xml::list( "effect", *this, &MIL_NbcAgentType::ReadEffect, data );

    const double total = std::accumulate( data.begin(), data.end(), 0. );
    if( std::fabs( 1. - total ) > 0.01 )
        xis.error( "Sum of poisonous percentage is out of bound for NBC Agent type" );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ReadEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::ReadEffect( xml::xistream& xis, T_HumanPoisonousVector& data )
{
    std::string wound;
    xis >> xml::attribute( "wound", wound );
    PHY_HumanWound::CIT_HumanWoundMap it = PHY_HumanWound::GetHumanWounds().find( wound );
    if( it != PHY_HumanWound::GetHumanWounds().end() )
    {
        double percentage;
        xis >> xml::attribute( "percentage", percentage );
        if( percentage < 0.f || percentage > 1.f )
            xis.error( "Poisonous percentage is out of bound for NBC Agent type" );
        data[ it->second->GetID() ] = percentage;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetRandomWound
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_NbcAgentType::GetRandomWound( const T_HumanPoisonousVector& data ) const
{
    const double rRand = MIL_Random::rand_ii( MIL_Random::eWounds );

    double rSumCoefs = 0.;
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

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::InitializePopulationEffect
// Created: LGY 2011-04-01
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::InitializePopulationEffect( MIL_IntoxicationEffect& effect ) const
{
    if( bLiquidPoisonous_ )
        ApplyIntoxication( effect, liquidPoisonous_ );
    if( bGasPoisonous_ )
        ApplyIntoxication( effect, gasPoisonous_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::ApplyIntoxication
// Created: LGY 2011-04-01
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::ApplyIntoxication( MIL_IntoxicationEffect& effect, const T_HumanPoisonousVector& /*data*/ ) const
{
    double wounded = liquidPoisonous_[ PHY_HumanWound::woundedU3_.GetID() ] + liquidPoisonous_[ PHY_HumanWound::woundedU2_.GetID() ]
                   + liquidPoisonous_[ PHY_HumanWound::woundedU1_.GetID() ] + liquidPoisonous_[ PHY_HumanWound::woundedUE_.GetID() ];
    double killed = liquidPoisonous_[ PHY_HumanWound::killed_.GetID() ];
    effect.Add( wounded, killed );
}
