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
#include "WeaponSystemType.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EquipmentType constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
EquipmentType::EquipmentType( xml::xistream& xis, const Resolver_ABC< WeaponSystemType, std::string >& weapons )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::start( "weapon-systems" )
            >> xml::list( "weapon-system", *this, &EquipmentType::ReadWeaponSystem, weapons )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: EquipmentType destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
EquipmentType::~EquipmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetName
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
std::string EquipmentType::GetName() const
{
    return name_;
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
// Name: EquipmentType::ReadWeaponSystem
// Created: SBO 2008-08-07
// -----------------------------------------------------------------------------
void EquipmentType::ReadWeaponSystem( xml::xistream& xis, const Resolver_ABC< WeaponSystemType, std::string >& weapons )
{
    std::string launcher, munition;
    xis >> xml::attribute( "launcher", launcher )
        >> xml::attribute( "munition", munition );
    const std::string systemId = launcher + munition; // $$$$ SBO 2008-08-06: 
    WeaponSystemType& type = weapons.Get( systemId );
    Register( type.GetId(), type );
}
