// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DecisionalModel.h"
#include "Mission.h"
#include "FragOrder.h"
#include "FragOrderType.h"
#include "MissionFactory.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DecisionalModel constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::DecisionalModel( xml::xistream& xis,
                                  const MissionFactory& factory,
                                  const T_Resolver& missionResolver,
                                  const tools::StringResolver< FragOrderType >& fragOrders )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::optional
            >> xml::start( "missions" )
                >> xml::list( "mission", *this, &DecisionalModel::ReadMission, factory, missionResolver )
            >> xml::end
        >> xml::optional
            >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &DecisionalModel::ReadFragOrder, factory, fragOrders )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::~DecisionalModel()
{
    tools::Resolver< Mission >::DeleteAll();
    tools::Resolver< FragOrder >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& DecisionalModel::GetName() const
{
    return name_;
}

namespace
{
    void AddFragOrder( const MissionFactory& factory,
                       Mission& mission,
                       const std::string& name )
    {
        FragOrder* order = factory.CreateFragOrder( name );
        if( ! mission.Find( order->GetId() ) )
            mission.Register( order->GetId(), *order );
        else
            delete order;
    }
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadMission
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void DecisionalModel::ReadMission( xml::xistream& xis,
                                   const MissionFactory& factory,
                                   const T_Resolver& missionResolver )
{
    Mission* mission = (factory.*missionResolver)( xis.attribute< std::string >( "name" ) );
    tools::Resolver< Mission >::Register( mission->GetId(), *mission );
    xis >> xml::list( "fragorder", [&]( xml::xistream& fragOrderXis ) {
            AddFragOrder( factory, *mission, fragOrderXis.attribute< std::string >( "name" ) );
        } );
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadFragOrder
// Created: HBD 2010-08-25
// -----------------------------------------------------------------------------
void DecisionalModel::ReadFragOrder( xml::xistream& xis,
                                     const MissionFactory& factory,
                                     const tools::StringResolver< FragOrderType >& fragOrders )
{
    auto name = xis.attribute< std::string >( "name" );
    if( fragOrders.Get( name ).IsMissionRequired() )
    {
        tools::Resolver< Mission >::Apply( [&] ( Mission& mission ) {
            AddFragOrder( factory, mission, name );
        } );
    }
    else
    {
        FragOrder* order = factory.CreateFragOrder( name );
        tools::Resolver< FragOrder >::Register( order->GetId(), *order );
    }
}
