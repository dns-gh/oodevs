// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MagicActionType.h"
#include "protocol/Protocol.h"
#include "Karma.h"
#include "OrderParameter.h"
#include "Tools.h"
#include "protocol/Actions.h"

using namespace kernel;
using namespace protocol;

namespace
{

// So many different ways to do the same thing
std::string DiplomacyToString( int e )
{
    switch( static_cast< sword::EnumDiplomacy >( e ) )
    {
        case sword::unknown: return Karma::unknown_.GetName().toStdString();
        case sword::friendly: return Karma::friend_.GetName().toStdString();
        case sword::enemy: return Karma::enemy_.GetName().toStdString();
        case sword::neutral: return Karma::neutral_.GetName().toStdString();
    }
    return "";
}

std::string IdentificationToString( int e )
{
    switch( static_cast< sword::UnitIdentification_Level >( e ) )
    {
        case sword::UnitIdentification::detected:
            return tools::ToString( eDetection ).toStdString();
        case sword::UnitIdentification::recognized:
            return tools::ToString( eRecognition ).toStdString();
        case sword::UnitIdentification::identified:
            tools::ToString( eIdentification ).toStdString();
    }
    return "";
}

std::string WeatherToString( int e )
{
    switch( static_cast< E_WeatherType >( e ) )
    {
        case eWeatherType_None:      return "PasDePrecipitation";
        case eWeatherType_SandStorm: return "TempeteDeSable";
        case eWeatherType_Fog:       return "Brouillard";
        case eWeatherType_Drizzle:   return "Crachin";
        case eWeatherType_Rain:      return "Pluie";
        case eWeatherType_Snow:      return "Neige";
        case eWeatherType_Smoke:     return "Fumigene";
    };
    return "";
}

struct EnumStringifier
{
    const char* name;
    boost::function< std::string( int ) > fn;
};

template< typename E >
std::string EnumToString( int e )
{
    return tools::ToString( static_cast< E >( e ) ).toStdString();
}

const EnumStringifier stringifiers[] =
{
    { "tiredness", EnumToString< E_UnitTiredness > },
    { "moral", EnumToString< E_UnitMorale > },
    { "experience", EnumToString< E_UnitExperience > },
    { "stress", EnumToString< E_UnitStress > },
    { "attitude", EnumToString< E_PopulationAttitude > },
    { "identification", EnumToString< E_PopulationAttitude > },
    { "diplomacy", DiplomacyToString },
    { "identification", IdentificationToString },
    { "weather", WeatherToString },
};
const size_t stringifiersCount = sizeof( stringifiers )/sizeof( *stringifiers );

}  // namespace

const boost::function< std::string( int ) >& kernel::GetEnumStringifier( const std::string& name )
{
    for( size_t i = 0; i != stringifiersCount; ++i )
        if( stringifiers[i].name == name )
            return stringifiers[i].fn;
    throw MASA_EXCEPTION( "unknown enumeration " + name );
}

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType()
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType( const std::string& name, unsigned long id )
    : OrderType( name, id )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: FDS 2010-11-23
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType( const std::string& name )
    : OrderType( name, 0 )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::~MagicActionType()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::CreateOrderParameter
// Created: JSR 2010-04-23
// -----------------------------------------------------------------------------
OrderParameter* MagicActionType::CreateOrderParameter( const std::string& name, const std::string& type )
{
    OrderParameter* param = new OrderParameter( name, type, false );
    Register( Count(), *param );
    return param;
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::Initialize
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void MagicActionType::Initialize()
{
    const std::string& name = GetName();
    for( size_t i = 0; i != mapping::actionsCount; ++i )
    {
        const mapping::Action& action = mapping::actions[i];
        if( action.name != name )
            continue;
        for( size_t j = 0; j != action.paramsCount; ++j )
        {
            const auto& p = action.params[j];
            OrderParameter* param = CreateOrderParameter( p.name, p.type );
            if( !p.enumeration )
                continue;
            const auto& e = *p.enumeration;
            const auto& stringifier = GetEnumStringifier( e.name );
            for( size_t k = 0; k != e.valuesCount; ++k )
            {
                const int value = e.values[k];
                const std::string s = stringifier( value );
                if( s.empty() )
                    continue;
                param->AddValue( value, s );
            }
        }
    }
}
