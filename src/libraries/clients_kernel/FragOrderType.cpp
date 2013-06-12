// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FragOrderType.h"
#include "Entity_ABC.h"
#include "OrderParameter.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragOrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
FragOrderType::FragOrderType( xml::xistream& xis )
    : OrderType( xis )
    , isWithoutMission_( false )
{
    xis >> xml::optional >> xml::attribute( "available-without-mission", isWithoutMission_ )
        >> xml::optional >> xml::attribute( "dia-type", diaType_ )
        >> xml::list( "parameter", *this, &FragOrderType::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: FragOrderType destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
FragOrderType::~FragOrderType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::IsMissionRequired
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
bool FragOrderType::IsMissionRequired() const
{
    return !isWithoutMission_;
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::ReadParameter
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void FragOrderType::ReadParameter( xml::xistream& xis )
{
    OrderParameter* param = new OrderParameter( xis );
    Register( Count(), *param );
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::IsAvailableFor
// Created: SBO 2010-06-23
// -----------------------------------------------------------------------------
bool FragOrderType::IsAvailableFor( const kernel::Entity_ABC& entity ) const
{
    const std::string& typeName = entity.GetTypeName();
    std::string repOrderPion ( "Rep_OrderConduite_Pion_" );
    std::string repOrderAutomat ( "Rep_OrderConduite_Automate_"  );
    std::string repOrderPopulation ( "Rep_OrderConduite_Population_"  );
    if( 0 == diaType_.compare( 0, repOrderPion.size() ,repOrderPion ) )
        return typeName == "agent";
    if( 0 == diaType_.compare( 0, repOrderAutomat.size(), repOrderAutomat ) )
        return typeName == "automat";
    if( 0 == diaType_.compare( 0, repOrderPopulation.size(), repOrderPopulation) )
        return typeName == "crowd";
    return true;
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::GetType
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
E_MissionType FragOrderType::GetType() const
{
    return eMissionType_FragOrder;
}
