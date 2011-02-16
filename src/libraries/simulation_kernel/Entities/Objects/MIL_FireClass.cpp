// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FireClass.h"
#include "UrbanType.h"
#include <urban/StaticModel.h>
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "meteo/PHY_Precipitation.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.hpp>

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
        throw std::runtime_error( "Fire Class " + strName + " already exists" );
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
        >> xml::end
        >> xml::start( "urban-modifiers" )
            >> xml::list( "urban-modifier", *this, &MIL_FireClass::ReadUrbanModifier )
        >> xml::end
        >> xml::start( "surfaces" )
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
        xis.error( "Unknow extinguisher agent " + agent );

    for( T_ExtinguisherAgentEffectVector::const_iterator it = extinguisherAgentEffects_.begin(); it != extinguisherAgentEffects_.end(); ++it )
        if( it->pExtinguisherAgent_ == effect.pExtinguisherAgent_ )
            xis.error( "Duplicate extinguisher agent data: " + agent );

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
        throw std::runtime_error( "Unknow weather " + weather );

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
        xis.error( "Unknow injury type " + type );

    injuries_.insert( std::make_pair( pType, injury ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadUrbanModifier
// Created: BCI 2010-12-08
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadUrbanModifier( xml::xistream& xis )
{
    UrbanModifier urbanModifier;
    std::string materialType;
    xis >> xml::attribute( "material-type", materialType )
        >> xml::attribute( "value", urbanModifier.factor_ );

    urban::MaterialCompositionType* pMaterial = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( materialType );
    if( !pMaterial )
        xis.error( "Unknow material type : " + materialType );

    urbanModifiers_.insert( std::make_pair( pMaterial, urbanModifier ) );
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

    const TerrainData data = MIL_Tools::ConvertLandType( strTerrainType );
    if( data.Area() == 0xFF )
        xis.error( "Unknown terrain type '" + strTerrainType + "'" );

    surfaces_.insert( std::make_pair( data.Area(), surface ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Terminate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::Terminate()
{
    for( CIT_FireClassMap it = classes_.begin(); it != classes_.end(); ++it )
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
    CIT_FireClassMap it = classes_.find( strName );
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
        throw std::runtime_error( "No default MIL_FireClass available" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetWeatherDecreateRate
// Created: BCI 2010-12-13
// -----------------------------------------------------------------------------
int MIL_FireClass::GetWeatherDecreateRate( const weather::PHY_Precipitation& precipitation ) const
{
    T_WeatherEffectMap::const_iterator it = weatherEffects_.find( &precipitation );
    if( it != weatherEffects_.end() )
        return it->second.heatDecreaseRate_;
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ChooseRandomWound
// Created: BCI 2010-12-14
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_FireClass::ChooseRandomWound() const
{
    const double rRand = MIL_Random::rand_oi( MIL_Random::eWounds );

    double rSumFactors = 0.;
    for( T_InjuryMap::const_iterator it = injuries_.begin(); it != injuries_.end(); ++it )
    {
        const PHY_HumanWound& wound = *it->first;
        rSumFactors += it->second.percentage_;
        if( rSumFactors >= rRand )
            return wound;
    }
    throw std::runtime_error( __FUNCTION__ ": cannot choose random fire wound. Check if the injuries of fire class " + name_ + " are valid" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetSurfaceModifier
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_FireClass::GetSurfaceFirePotentials( const TerrainData& terrainData, int& ignitionThreshold, int& maxCombustionEnergy ) const
{
    ignitionThreshold = 0;
    maxCombustionEnergy = std::numeric_limits< int >::max();
    for( T_SurfaceMap::const_iterator it = surfaces_.begin(); it != surfaces_.end(); ++it )
    {
        if( it->first & terrainData.Area() )
        {
            ignitionThreshold = std::max( ignitionThreshold, it->second.ignitionThreshold_ );
            maxCombustionEnergy = std::min( maxCombustionEnergy, it->second.maxCombustionEnergy_ );
        }
    }
    if( maxCombustionEnergy == std::numeric_limits< int >::max() )
        maxCombustionEnergy = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::function< bool
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
const PHY_DotationCategory* MIL_FireClass::FindBestExtinguisherAgent( boost::function< bool( const PHY_DotationCategory& ) > isExtinguisherAgentOkFun ) const
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
