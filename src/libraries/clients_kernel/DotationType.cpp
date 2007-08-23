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
#include "xeumeuleu/xml.h"
#include "tools.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
DotationType::DotationType( xistream& xis )
    : dType_( false )
{
    std::string name, category;
    xis >> attribute( "id", id_ )
        >> attribute( "name", name )
        >> attribute( "category", category )
        >> optional() >> attribute( "d-type", dType_ );
    name_ = name.c_str(); category_ = category.c_str();
    nameId_ = tools::DotationFamilyFromString( name_.ascii() );
    gaz_        = ( name_ == "carburant" );
    ammunition_ = ( name_ == "munition" );
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
QString DotationType::GetName() const
{
    return name_;
}
    
// -----------------------------------------------------------------------------
// Name: DotationType::GetCategory
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
QString DotationType::GetCategory() const
{
    return category_;
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
// Name: DotationType::GetFamily
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
unsigned long DotationType::GetFamily() const
{
    return nameId_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::IsDType
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool DotationType::IsDType() const
{
    return dType_;
}
