// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EquipmentType.h"
#include "BreakdownOriginType.h"
#include "DotationCapacityType.h"
#include "WeaponSystemType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EquipmentType constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
EquipmentType::EquipmentType( xml::xistream& xis, const tools::Resolver_ABC< WeaponSystemType, std::string >& weapons )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "protection", protection_ )
        >> xml::start( "weapon-systems" )
            >> xml::list( "weapon-system", *this, &EquipmentType::ReadWeaponSystem, weapons )
        >> xml::end
        >> xml::optional >> xml::start( "breakdowns" )
            >> xml::list( "breakdown", *this, &EquipmentType::ReadBreakdown )
        >> xml::end
        >> xml::optional >> xml::start( "composition" )
            >> xml::list( "category", *this, &EquipmentType::ReadResourceCategory )
        >> xml::end
        >> xml::optional >> xml::start( "logistic-functions" )
            >> xml::optional >> xml::start( "supply-functions" )
                >> xml::list( "carrying", *this, &EquipmentType::ReadLogSupplyFunction )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: EquipmentType destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
EquipmentType::~EquipmentType()
{
    for( CIT_Breakdowns it = breakdowns_.begin(); it != breakdowns_.end(); ++it )
        delete *it;
    for( CIT_Resources it = resources_.begin(); it != resources_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetName
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
const std::string& EquipmentType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetProtection
// Created: JSR 2011-02-10
// -----------------------------------------------------------------------------
const std::string& EquipmentType::GetProtection() const
{
    return protection_;
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetId
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
unsigned long EquipmentType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const BreakdownOriginType& > EquipmentType::CreateBreakdownsIterator
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
tools::Iterator< const BreakdownOriginType& > EquipmentType::CreateBreakdownsIterator() const
{
    return new tools::SimpleIterator< const BreakdownOriginType&, T_Breakdowns >( breakdowns_ );
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const DotationCapacityType& > EquipmentType::CreateResourcesIterator
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
tools::Iterator< const DotationCapacityType& > EquipmentType::CreateResourcesIterator() const
{
    return new tools::SimpleIterator< const DotationCapacityType&, T_Resources >( resources_ );
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::ReadWeaponSystem
// Created: SBO 2008-08-07
// -----------------------------------------------------------------------------
void EquipmentType::ReadWeaponSystem( xml::xistream& xis, const tools::Resolver_ABC< WeaponSystemType, std::string >& weapons )
{
    std::string launcher, munition;
    xis >> xml::attribute( "launcher", launcher )
        >> xml::attribute( "munition", munition );
    const std::string systemId = launcher + munition; // $$$$ SBO 2008-08-06:
    WeaponSystemType& type = weapons.Get( systemId );
    Register( type.GetId(), type );
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::ReadBreakdown
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void EquipmentType::ReadBreakdown( xml::xistream& xis )
{
    breakdowns_.push_back( new BreakdownOriginType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::ReadResourceCategory
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void EquipmentType::ReadResourceCategory( xml::xistream& xis )
{
    xis >> xml::list( "resource", *this, &EquipmentType::ReadResource );
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::ReadResource
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void EquipmentType::ReadResource( xml::xistream& xis )
{
    resources_.push_back( new DotationCapacityType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::ReadLogSupplyFunction
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void EquipmentType::ReadLogSupplyFunction( xml::xistream& xis )
{
    carryingSupplyFunction_.reset( new CarryingSupplyFunction( xis.attribute< std::string >( "nature" ), xis.attribute< double >( "mass" ), xis.attribute< double >( "volume" ) ) );
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetLogSupplyFunctionCarrying
// Created: JSR 2012-01-03
// -----------------------------------------------------------------------------
const EquipmentType::CarryingSupplyFunction* EquipmentType::GetLogSupplyFunctionCarrying() const
{
    return carryingSupplyFunction_.get();
}