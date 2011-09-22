// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "InhabitantType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InhabitantType constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantType::InhabitantType( xml::xistream& xis, const tools::Resolver_ABC< PopulationType, std::string >& modelResolver )
    : male_    ( 0 )
    , female_  ( 0 )
    , children_( 0 )
{
    std::string model;
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "associated-crowd", model );
    type_ = &modelResolver.Get( model );
    xis >> xml::start( "repartition" )
            >> xml::attribute( "male", male_ )
            >> xml::attribute( "female", female_ )
            >> xml::attribute( "children", children_ )
        >> xml::end
        >> xml::start( "consumption" )
            >> xml::list( "resource", *this, &InhabitantType::ReadConsumption )
        >> xml::end
        >> xml::start( "health-need" )
            >> xml::attribute( "people-per-facility", healthNeed_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantType::~InhabitantType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetName
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
const std::string& InhabitantType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetId
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
unsigned long InhabitantType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetCrowdType
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
const PopulationType& InhabitantType::GetCrowdType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetMalePercentage
// Created: SLG 2010-12-07
// -----------------------------------------------------------------------------
float InhabitantType::GetMalePercentage() const
{
    return male_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetFemalePercentage
// Created: SLG 2010-12-07
// -----------------------------------------------------------------------------
float InhabitantType::GetFemalePercentage() const
{
    return female_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetChildrenPercentage
// Created: SLG 2010-12-07
// -----------------------------------------------------------------------------
float InhabitantType::GetChildrenPercentage() const
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetHealthNeed
// Created: JSR 2011-02-07
// -----------------------------------------------------------------------------
unsigned int InhabitantType::GetHealthPeopleNumber() const
{
    return healthNeed_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetConsumption
// Created: JSR 2011-09-21
// -----------------------------------------------------------------------------
double InhabitantType::GetConsumption( const std::string& resource ) const
{
    std::map< std::string, double >::const_iterator it = consumptions_.find( resource);
    if( it == consumptions_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::ReadConsumption
// Created: JSR 2011-09-21
// -----------------------------------------------------------------------------
void InhabitantType::ReadConsumption( xml::xistream& xis )
{
    consumptions_[ xis.attribute< std::string >( "type" ) ] = xis.attribute< double >( "need" );
}
