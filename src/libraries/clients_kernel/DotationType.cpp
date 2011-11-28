// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DotationType.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
DotationType::DotationType( xml::xistream& xis, const tools::Resolver_ABC< LogisticSupplyClass, std::string >& resolver )
{
    std::string category;
    std::string strLogisticSupplyClass;
    xis >> xml::attribute( "id", id_ )
        >> xml::attribute( "name", name_ )
        >> xml::attribute( "category", category )
        >> xml::attribute( "logistic-supply-class", strLogisticSupplyClass )
        >> xml::optional >> xml::attribute( "type", type_ );
    logisticSupplyClass_ = &resolver.Get( strLogisticSupplyClass );
    categoryId_ = tools::DotationFamilyFromString( category );
    gaz_        = ( category == "carburant" );
    ammunition_ = ( category == "munition" );
    indirectFireAmmunition_ = xis.has_child( "indirect-fire" );
}

// -----------------------------------------------------------------------------
// Name: DotationType destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
DotationType::~DotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetId
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
unsigned long DotationType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetName
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
const std::string& DotationType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetCategoryDisplay
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
const std::string DotationType::GetCategoryDisplay() const
{
    return tools::ToString( static_cast< E_DotationFamily >( categoryId_ ), ENT_Tr_ABC::eToTr ).ascii();
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetCategory
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
const std::string DotationType::GetCategoryName() const
{
    return tools::ToString( static_cast< E_DotationFamily >( categoryId_ ), ENT_Tr_ABC::eToSim ).ascii();
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetType
// Created: JSR 2011-02-10
// -----------------------------------------------------------------------------
const std::string& DotationType::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::IsGas
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool DotationType::IsGas() const
{
    return gaz_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::IsAmmunition
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool DotationType::IsAmmunition() const
{
    return ammunition_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::IsIndirectFireAmmunition
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool DotationType::IsIndirectFireAmmunition() const
{
    return indirectFireAmmunition_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetFamily
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
unsigned long DotationType::GetFamily() const
{
    return categoryId_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetLogisticSupplyClass
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
const LogisticSupplyClass& DotationType::GetLogisticSupplyClass() const
{
    assert( logisticSupplyClass_ );
    return *logisticSupplyClass_;
}
