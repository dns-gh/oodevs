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
#include "MissionFactory.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: DecisionalModel constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::DecisionalModel( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver, const Resolver_ABC< FragOrder >& fragOrders )
{
    std::string name;
    xis >> attribute( "nom", name )
        >> optional() 
            >> start( "Missions" )
			>> list( "Mission", *this, &DecisionalModel::ReadMission, factory, missionResolver )
            >> end();
    name_ = name.c_str();
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
    xis >> attribute( "nom", name );
    Mission* mission = (factory.*missionResolver)( name );
    Resolver< Mission >::Register( mission->GetId(), *mission );

    xis >> optional() 
        >> start( "OrdresConduite" )
            >> list( "OrdreConduite", *this, &DecisionalModel::ReadFragOrder, *mission, factory )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadFragOrder
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void DecisionalModel::ReadFragOrder( xml::xistream& xis, Mission& mission, MissionFactory& factory )
{
    std::string name;
    xis >> attribute( "nom", name );
    FragOrder* order = factory.CreateFragOrder( name );
    if( ! mission.Find( order->GetId() ) )
        mission.Register( order->GetId(), *order );
    else 
        delete order;
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
QString DecisionalModel::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::RegisterDefaultFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void DecisionalModel::RegisterDefaultFragOrders( MissionFactory& factory, const Resolver_ABC< FragOrder >& fragOrders )
{
    Iterator< const FragOrder& > it = fragOrders.CreateIterator();
    while( it.HasMoreElements() )
    {
        const FragOrder& order = it.NextElement();
        if( order.IsDefaultOrder() )
            RegisterFragOrder( factory, order.GetName().ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::RegisterFragOrder
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void DecisionalModel::RegisterFragOrder( MissionFactory& factory, const std::string& name )
{
    FragOrder* order = factory.CreateFragOrder( name );
    Resolver< FragOrder >::Register( order->GetId(), *order );
}


