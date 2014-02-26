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
#include "MIL_Random.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"
#include "MT_Tools/MT_Logger.h"
#include <numeric>

MIL_NbcAgentType::T_NbcAgentTypeMap MIL_NbcAgentType::nbcAgentTypes_;
double MIL_NbcAgentType::rCoefMaxSpeedModificator_      = 1.;
double MIL_NbcAgentType::rCoefReloadingTimeModificator_ = 1.;
double MIL_NbcAgentType::rContaminationDistance_        = 100.;
double MIL_NbcAgentType::rContaminationQuantityGiven_   = 0.5;
double MIL_NbcAgentType::rMinPropagationSpeed_          = 0.;

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
                >> xml::attribute( "wind-speed-limit", rMinPropagationSpeed_ )
            >> xml::end;

    if( rCoefMaxSpeedModificator_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "nbc-suit: max-speed-modifier < 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "nbc-suit: reloading-time-modifier <= 0" );

    if( rContaminationDistance_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "propagation: contamination-distance < 0" );

    xis >> xml::start( "agents" )
            >> xml::list( "agent", &MIL_NbcAgentType::ReadAgent )
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
        throw MASA_EXCEPTION( xis.context() + "Unknown NBC agent type" );
    pNbcAgentType = new MIL_NbcAgentType( strName, xis );

    if( !ids_.insert( pNbcAgentType->GetID() ).second )
        throw MASA_EXCEPTION( xis.context() + "NBC agent type ID already used" );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::Terminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NbcAgentType::Terminate()
{
    for( auto it = nbcAgentTypes_.begin(); it != nbcAgentTypes_.end(); ++it )
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
        throw MASA_EXCEPTION( "effects: unknown type" );
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
    std::string nGasLifeTime = "6h";
    xis >> xml::optional >> xml::attribute( "contamination", bGasContaminating_ )
        >> xml::optional >> xml::attribute( "life-time", nGasLifeTime );
    tools::DecodeTime( nGasLifeTime, nGasLifeTime_ );
    nGasLifeTime_ = MIL_Tools::ConvertSecondsToSim( nGasLifeTime_ );
    if( nGasLifeTime_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "effects: life-time <= 0" );

    xis >> xml::attribute( "propagation", rGasPropagationAngle_ );
    if( rGasPropagationAngle_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "effects: propagation <= 0" );
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
        throw MASA_EXCEPTION( xis.context() + "Sum of poisonous percentage is out of bound for NBC Agent type" );
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
    auto it = PHY_HumanWound::GetHumanWounds().find( wound );
    if( it != PHY_HumanWound::GetHumanWounds().end() )
    {
        double percentage;
        xis >> xml::attribute( "percentage", percentage );
        if( percentage < 0.f || percentage > 1.f )
            throw MASA_EXCEPTION( xis.context() + "Poisonous percentage is out of bound for NBC Agent type" );
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
    const auto& humanWounds = PHY_HumanWound::GetHumanWounds();
    for( auto itWound = humanWounds.begin(); itWound != humanWounds.end(); ++itWound )
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
void MIL_NbcAgentType::ApplyIntoxication( MIL_IntoxicationEffect& effect, const T_HumanPoisonousVector& data ) const
{
    double wounded = data[ PHY_HumanWound::woundedU3_.GetID() ] + data[ PHY_HumanWound::woundedU2_.GetID() ]
                   + data[ PHY_HumanWound::woundedU1_.GetID() ] + data[ PHY_HumanWound::woundedUE_.GetID() ];
    double killed = data[ PHY_HumanWound::killed_.GetID() ];
    effect.Add( wounded, killed );
}

const MIL_NbcAgentType* MIL_NbcAgentType::Find( const std::string& strName )
{
    CIT_NbcAgentTypeMap it = nbcAgentTypes_.find( strName );
    if( it == nbcAgentTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::Find
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
const MIL_NbcAgentType* MIL_NbcAgentType::Find( unsigned int nID )
{
    for( CIT_NbcAgentTypeMap itNbcAgent = nbcAgentTypes_.begin(); itNbcAgent != nbcAgentTypes_.end(); ++itNbcAgent )
    {
        const MIL_NbcAgentType& nbcAgentType = *itNbcAgent->second;
        if( nbcAgentType.GetID() == nID )
            return &nbcAgentType;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::CanBeVaporized
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::CanBeVaporized() const
{
    return bCanBeVaporized_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetName
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
const std::string& MIL_NbcAgentType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetID
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
unsigned int MIL_NbcAgentType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetLiquidRandomWound
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_NbcAgentType::GetLiquidRandomWound() const
{
    return GetRandomWound( liquidPoisonous_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetGasRandomWound
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_NbcAgentType::GetGasRandomWound() const
{
    return GetRandomWound( gasPoisonous_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsGasContaminating
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::IsGasContaminating() const
{
    return bGasContaminating_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsLiquidContaminating
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::IsLiquidContaminating() const
{
    return bLiquidContaminating_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsGasPoisonous
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::IsGasPoisonous() const
{
    return bGasPoisonous_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsLiquidPoisonous
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
bool MIL_NbcAgentType::IsLiquidPoisonous() const
{
    return bLiquidPoisonous_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetGasLifeTime
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetGasLifeTime() const
{
    return nGasLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetGasPropagationAngle
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetGasPropagationAngle() const
{
    return rGasPropagationAngle_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetCoefMaxSpeedModificator
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetCoefMaxSpeedModificator()
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetCoefReloadingTimeModificator
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetCoefReloadingTimeModificator()
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetContaminationDistance
// Created: HBD 2010-06-08
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetContaminationDistance()
{
    return rContaminationDistance_;
}
// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetMinContaminationQuantity
// Created: HBD 2010-06-08
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetMinContaminationQuantity()
{
    return rContaminationQuantityGiven_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetMinPropagationSpeed
// Created: LGY 2011-07-05
// -----------------------------------------------------------------------------
double MIL_NbcAgentType::GetMinPropagationSpeed()
{
    return rMinPropagationSpeed_;
}
