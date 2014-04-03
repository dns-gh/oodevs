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

#include <boost/optional.hpp>

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
    T_Stringifier fn;
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
    { "injury", EnumToString< E_InjuriesSeriousness > },
    { "human_rank", EnumToString< E_HumanRank > },
    { "human_state", EnumToString< E_HumanState > },
    { "diplomacy", DiplomacyToString },
    { "identification", IdentificationToString },
    { "weather", WeatherToString },
};
const size_t stringifiersCount = sizeof( stringifiers )/sizeof( *stringifiers );

}  // namespace

T_Stringifier kernel::GetEnumStringifier( const std::string& name )
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

namespace
{
    void InitializeAction( tools::Resolver< OrderParameter >& parent,
                           const protocol::mapping::Action& action );
    void InitializeParam( tools::Resolver< OrderParameter >& parent,
                          const protocol::mapping::ActionParam& param );

    OrderParameter* CreateOrderParameter( tools::Resolver< OrderParameter >& parent,
                                          const std::string& name,
                                          const std::string& type,
                                          boost::optional< int > id = boost::none )
    {
        OrderParameter* param = new OrderParameter( name, type, false );
        parent.Register( id ? *id : parent.Count(), *param );
        return param;
    }

    void InitializeEnum( OrderParameter& orderParameter,
                         const protocol::mapping::ActionParam& param )
    {
        if( !param.enumeration )
            return;
        const auto& e = *param.enumeration;
        const auto& stringifier = GetEnumStringifier( e.name );
        for( size_t k = 0; k != e.valuesCount; ++k )
        {
            const int value = e.values[k];
            const std::string s = stringifier( value );
            if( !s.empty() )
                orderParameter.AddValue( value, s );
        }
    }

    void InitializeUnion( OrderParameter& orderParameter,
                          const protocol::mapping::ActionParam& param )
    {
        orderParameter.SetUnion( true );
        const auto& u = *param.structure_union;
        for( size_t j = 0; j != u.typesCount; ++j )
        {
            const auto& type = u.types[j];
            OrderParameter* typeParameter = CreateOrderParameter( orderParameter, type.name, "list", type.id );
            typeParameter->SetStructure( true );
            for( size_t k = 0; k != type.paramsCount; ++k )
            {
                const auto& subParam = type.params[k];
                InitializeParam( *typeParameter, subParam );
            }
        }
    }

    void InitializeParam( tools::Resolver< OrderParameter >& parent,
                          const protocol::mapping::ActionParam& param )
    {
        OrderParameter* orderParameter = CreateOrderParameter( parent, param.name, param.type );
        if( param.list )
            InitializeAction( *orderParameter, *param.list );
        else if( param.structure )
        {
            orderParameter->SetStructure( true );
            InitializeAction( *orderParameter, *param.structure );
        }
        else if( param.enumeration )
            InitializeEnum( *orderParameter, param );
        else if( param.structure_union )
            InitializeUnion( *orderParameter, param );
    }

    void InitializeAction( tools::Resolver< OrderParameter >& parent,
                           const protocol::mapping::Action& action )
    {
        for( size_t i = 0; i != action.paramsCount; ++i )
            InitializeParam( parent, action.params[i] );
    }
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
        InitializeAction( *this, action );
    }
}
