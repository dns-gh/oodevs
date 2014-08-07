// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FireClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Weapons/PHY_UrbanAttritionData.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "MIL_Random.h"
#include "meteo/PHY_Precipitation.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "Urban/PHY_MaterialCompositionType.h"

MIL_FireClass::T_FireClassMap MIL_FireClass::classes_;
unsigned int MIL_FireClass::cellSize_;

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadClass
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadFireClasses( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );
    const MIL_FireClass*& pClass = classes_[ strName ];
    if( pClass )
        throw MASA_EXCEPTION( "Fire Class " + strName + " already exists" );
    pClass = new MIL_FireClass( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Initialize
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing fire classes" );
    xis >> xml::start( "fires" )
            >> xml::start( "cell-size" )
                >> xml::attribute( "value", cellSize_ )
            >> xml::end
            >> xml::list( "fire", &ReadFireClasses )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass constructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_FireClass::MIL_FireClass( const std::string& name, xml::xistream& xis )
    : name_              ( name )
    , maxHeat_              ( 0 )
    , increaseRate_         ( 0 )
    , decreaseRate_         ( 0 )
{
    xis >> xml::attribute( "decrease-rate"           , decreaseRate_ )
        >> xml::attribute( "increase-rate"           , increaseRate_ )
        >> xml::attribute( "initial-heat"            , initialHeat_ )
        >> xml::attribute( "max-heat"                , maxHeat_ )
        >> xml::start( "extinguisher-agents" )
            >> xml::list( "extinguisher-agent", *this, &MIL_FireClass::ReadExtinguisherAgent )
        >> xml::end
        >> xml::start( "weather-effects" )
            >> xml::list( "weather-effect", *this, &MIL_FireClass::ReadWeatherEffect )
        >> xml::end
        >> xml::start( "injuries" )
            >> xml::list( "injury", *this, &MIL_FireClass::ReadInjury )
        >> xml::end;
        ReadUrbanModifiers( xis );
        xis >> xml::optional >> xml::start( "surfaces" )
            >> xml::list( "surface", *this, &MIL_FireClass::ReadSurface )
        >> xml::end;

    std::sort( extinguisherAgentEffects_.begin(), extinguisherAgentEffects_.end(), boost::bind( &ExtinguisherAgentEffect::heatDecreaseRate_, _1 ) > boost::bind( &ExtinguisherAgentEffect::heatDecreaseRate_, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadExtinguisherAgent
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadExtinguisherAgent( xml::xistream& xis )
{
    std::string agent;
    ExtinguisherAgentEffect effect;
    xis >> xml::attribute( "agent", agent )
        >> xml::attribute( "heat-decrease-rate", effect.heatDecreaseRate_ );

    effect.pExtinguisherAgent_ = PHY_DotationType::FindDotationCategory( agent );
    if( !effect.pExtinguisherAgent_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown extinguisher agent " + agent );

    for( T_ExtinguisherAgentEffectVector::const_iterator it = extinguisherAgentEffects_.begin(); it != extinguisherAgentEffects_.end(); ++it )
        if( it->pExtinguisherAgent_ == effect.pExtinguisherAgent_ )
            throw MASA_EXCEPTION( xis.context() + "Duplicate extinguisher agent data: " + agent );

    extinguisherAgentEffects_.push_back( effect );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadWeatherEffect
// Created: BCI 2010-12-08
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadWeatherEffect( xml::xistream& xis )
{
    std::string weather;
    WeatherEffect effect;
    xis >> xml::attribute( "weather", weather )
        >> xml::attribute( "heat-decrease-rate", effect.heatDecreaseRate_ );
    const weather::PHY_Precipitation* pWeather = weather::PHY_Precipitation::FindPrecipitation( weather );
    if( !pWeather )
        throw MASA_EXCEPTION( "Unknown weather " + weather );
    weatherEffects_.insert( std::make_pair( pWeather, effect ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadInjury
// Created: BCI 2010-12-08
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadInjury( xml::xistream& xis )
{
    Injury injury;
    std::string type;
    xis >> xml::attribute( "type", type )
        >> xml::attribute( "percentage", injury.percentage_ );

    const PHY_HumanWound* pType = PHY_HumanWound::Find( type );
    if( !pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown injury type " + type );

    injuries_.insert( std::make_pair( pType, injury ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadUrbanModifier
// Created: BCI 2010-12-08
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadUrbanModifiers( xml::xistream& xis )
{
    xis >> xml::start( "urban-modifiers" );
    urbanModifiers_.reset( new PHY_UrbanAttritionData( xis ) );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadSurface
// Created: BCI 2010-12-08
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadSurface( xml::xistream& xis )
{
    Surface surface;
    std::string strTerrainType;
    xis >> xml::attribute( "type", strTerrainType )
        >> xml::attribute( "ignition-threshold", surface.ignitionThreshold_ )
        >> xml::attribute( "max-combustion-energy", surface.maxCombustionEnergy_ );

    surface.terrainData_ = TerrainData::FromString( strTerrainType );
    if( surface.terrainData_.Area() == 0xFF )
        throw MASA_EXCEPTION( xis.context() + "Unknown terrain type '" + strTerrainType + "'" );

    surfaces_.push_back( surface );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Terminate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::Terminate()
{
    for( auto it = classes_.begin(); it != classes_.end(); ++it )
        delete it->second;
    classes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const MIL_FireClass* MIL_FireClass::Find( const std::string& strName )
{
    auto it = classes_.find( strName );
    if( it == classes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicFireClassA destructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_FireClass::~MIL_FireClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetCellSize
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
int MIL_FireClass::GetCellSize()
{
    return cellSize_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const std::string& MIL_FireClass::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetInitialHeat
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int MIL_FireClass::GetInitialHeat() const
{
    return initialHeat_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetDecreaseRate
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int MIL_FireClass::GetDecreaseRate() const
{
    return decreaseRate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetIncreaseRate
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int MIL_FireClass::GetIncreaseRate() const
{
    return increaseRate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetMaxHeat
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int MIL_FireClass::GetMaxHeat() const
{
    return maxHeat_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetDefaultFireClass
// Created: BCI 2010-11-30
// -----------------------------------------------------------------------------
const MIL_FireClass* MIL_FireClass::GetDefaultFireClass()
{
    if( !classes_.empty() )
        return classes_.begin()->second;
    else
        throw MASA_EXCEPTION( "No default MIL_FireClass available" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetUrbanAttritionData
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
const PHY_UrbanAttritionData& MIL_FireClass::GetUrbanAttritionData() const
{
    return *urbanModifiers_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetWeatherDecreateRate
// Created: BCI 2010-12-13
// -----------------------------------------------------------------------------
int MIL_FireClass::GetWeatherDecreateRate( const weather::PHY_Precipitation& precipitation ) const
{
    auto it = weatherEffects_.find( &precipitation );
    if( it != weatherEffects_.end() )
        return it->second.heatDecreaseRate_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ChooseRandomWound
// Created: BCI 2010-12-14
// -----------------------------------------------------------------------------
const PHY_HumanWound* MIL_FireClass::ChooseRandomWound() const
{
    const double rRand = MIL_Random::rand_oi( MIL_Random::eWounds );
    double rSumFactors = 0;
    for( auto it = injuries_.begin(); it != injuries_.end(); ++it )
    {
        rSumFactors += it->second.percentage_;
        if( rSumFactors >= rRand )
            return it->first;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetSurfaceModifier
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_FireClass::GetSurfaceFirePotentials( const TerrainData& terrainData, int& ignitionThreshold, int& maxCombustionEnergy ) const
{
    ignitionThreshold = -1;
    maxCombustionEnergy = std::numeric_limits< int >::max();
    for( T_Surfaces::const_iterator it = surfaces_.begin(); it != surfaces_.end(); ++it )
    {
        if( it->terrainData_.ContainsOne( terrainData ) )
        {
            ignitionThreshold = std::max( ignitionThreshold, it->ignitionThreshold_ );
            maxCombustionEnergy = std::min( maxCombustionEnergy, it->maxCombustionEnergy_ );
        }
    }
    if( maxCombustionEnergy == std::numeric_limits< int >::max() )
        maxCombustionEnergy = 0;
    if( ignitionThreshold < 0 )
        ignitionThreshold = std::numeric_limits< int >::max();
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::function< bool
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
const PHY_DotationCategory* MIL_FireClass::FindBestExtinguisherAgent( std::function< bool( const PHY_DotationCategory& ) > isExtinguisherAgentOkFun ) const
{
    for( T_ExtinguisherAgentEffectVector::const_iterator it = extinguisherAgentEffects_.begin(); it != extinguisherAgentEffects_.end(); ++it )
        if( isExtinguisherAgentOkFun( *it->pExtinguisherAgent_ ) )
            return it->pExtinguisherAgent_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetExtinguisherHeatDecreaseRate
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
int MIL_FireClass::GetExtinguisherHeatDecreaseRate( const PHY_DotationCategory& extinguisherAgent ) const
{
    for( T_ExtinguisherAgentEffectVector::const_iterator it = extinguisherAgentEffects_.begin(); it != extinguisherAgentEffects_.end(); ++it )
            if( it->pExtinguisherAgent_ == &extinguisherAgent )
                return it->heatDecreaseRate_;
    return 0;
}
