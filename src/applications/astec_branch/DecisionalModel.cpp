// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "DecisionalModel.h"
#include "Mission.h"
#include "FragOrder.h"
#include "MissionFactory.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: DecisionalModel constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::DecisionalModel( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver )
{
    xis >> attribute( "nom", name_ )
        >> optional() 
            >> start( "Missions" )
                >> list( "Mission", *this, ReadMission, factory, missionResolver )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::~DecisionalModel()
{
    Resolver< Mission >::DeleteAll();
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
    Resolver< Mission >::Register( mission->id_, *mission );

    xis >> optional() 
        >> start( "OrdresConduite" )
            >> list( "OrdreConduite", *this, ReadFragOrder, *mission, factory )
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
    mission.AddAvailableOrder( *order );
    if( ! Resolver< FragOrder >::Find( order->id_ ) )
        Resolver< FragOrder >::Register( order->id_, *order );
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& DecisionalModel::GetName() const
{
    return name_;
}
