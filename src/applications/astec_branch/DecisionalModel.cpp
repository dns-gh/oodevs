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
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: DecisionalModel constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::DecisionalModel( xml::xistream& xis, const T_Resolver& missionResolver, const T_Resolver& fragOrderResolver )
{
    xis >> attribute( "nom", name_ )
        >> optional() 
            >> start( "Missions" )
                >> list( "Mission", *this, ReadMission, missionResolver, fragOrderResolver )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
DecisionalModel::~DecisionalModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadMission
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void DecisionalModel::ReadMission( xml::xistream& xis, const T_Resolver& missionResolver, const T_Resolver& fragOrderResolver )
{
    std::string mission;
    xis >> attribute( "nom", mission );
    availableMissions_.push_back( missionResolver( mission ) );

    xis >> optional() 
        >> start( "OrdresConduite" )
            >> list( "OrdreConduite", *this, ReadFragOrder, fragOrderResolver )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::ReadFragOrder
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void DecisionalModel::ReadFragOrder( xml::xistream& xis, const T_Resolver& resolver )
{
    std::string name;
    xis >> attribute( "nom", name );
    availableFragOrders_.insert( resolver( name ) );
}

// -----------------------------------------------------------------------------
// Name: DecisionalModel::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& DecisionalModel::GetName() const
{
    return name_;
}
