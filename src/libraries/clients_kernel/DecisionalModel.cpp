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
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: DecisionalModel constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::DecisionalModel( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver, const Resolver_ABC< FragOrderType >& fragOrders )
{
    xis >> attribute( "name", name_ )
        >> optional() 
            >> start( "missions" )
                >> list( "mission", *this, &DecisionalModel::ReadMission, factory, missionResolver )
            >> end();
    RegisterDefaultFragOrders( factory, fragOrders );
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::~DecisionalModel()
{
    Resolver< Mission >::DeleteAll();
    Resolver< FragOrder >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadMission
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void DecisionalModel::ReadMission( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver )
{
    std::string name;
    xis >> attribute( "name", name );
    Mission* mission = (factory.*missionResolver)( name );
    Resolver< Mission >::Register( mission->GetId(), *mission );
    xis >> list( "fragorder", *this, &DecisionalModel::ReadFragOrder, *mission, factory );
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadFragOrder
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void DecisionalModel::ReadFragOrder( xml::xistream& xis, Mission& mission, MissionFactory& factory )
{
    std::string name;
    xis >> attribute( "name", name );
    FragOrder* order = factory.CreateFragOrder( name );
    if( ! static_cast< Resolver< FragOrder >& >( mission ).Find( order->GetId() ) )
        static_cast< Resolver< FragOrder >& >( mission ).Register( order->GetId(), *order );
    else 
        delete order;
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
std::string DecisionalModel::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::RegisterDefaultFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void DecisionalModel::RegisterDefaultFragOrders( MissionFactory& factory, const Resolver_ABC< FragOrderType >& fragOrders )
{
    Iterator< const FragOrderType& > it = fragOrders.CreateIterator();
    while( it.HasMoreElements() )
    {
        const FragOrderType& type = it.NextElement();
        if( type.IsDefaultOrder() )
            RegisterFragOrder( factory, type );
    }
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::RegisterFragOrder
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void DecisionalModel::RegisterFragOrder( MissionFactory& factory, const FragOrderType& type )
{
    FragOrder* order = factory.CreateFragOrder( type.GetName() );
    Resolver< FragOrder >::Register( order->GetId(), *order );
}

