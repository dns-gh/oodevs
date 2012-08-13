// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
DotationType::DotationType( xml::xistream& xis )
    : dType_( false )
{
    std::string category;
    double nbrInPackage;
    xis >> xml::attribute( "id", id_ )
        >> xml::attribute( "name", name_ )
        >> xml::attribute( "category", category )
        >> xml::attribute( "package-size", nbrInPackage )
        >> xml::attribute( "package-mass", unitWeight_ )
        >> xml::attribute( "package-volume", unitVolume_ )
        >> xml::attribute( "nature", nature_ )
        >> xml::optional >> xml::attribute( "type", type_ )
        >> xml::optional >> xml::attribute( "d-type", dType_ );
    assert( nbrInPackage > 0 );
    if( nbrInPackage > 0 )
    {
        unitWeight_ /= nbrInPackage;
        unitVolume_ /= nbrInPackage;
    }
    categoryId_ = tools::DotationFamilyFromString( category );
    gaz_        = ( category == "carburant" );
    ammunition_ = ( category == "munition" );
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
    return tools::ToString( static_cast< E_DotationFamily >( categoryId_ ), ENT_Tr_ABC::eToTr ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetCategory
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
const std::string DotationType::GetCategoryName() const
{
    return tools::ToString( static_cast< E_DotationFamily >( categoryId_ ), ENT_Tr_ABC::eToSim ).toStdString();
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
// Name: DotationType::GetNature
// Created: JSR 2012-01-03
// -----------------------------------------------------------------------------
const std::string& DotationType::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetUnitWeight
// Created: JSR 2012-01-03
// -----------------------------------------------------------------------------
double DotationType::GetUnitWeight() const
{
    return unitWeight_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetUnitVolume
// Created: JSR 2012-01-03
// -----------------------------------------------------------------------------
double DotationType::GetUnitVolume() const
{
    return unitVolume_;
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
    return categoryId_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::IsDType
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool DotationType::IsDType() const
{
    return dType_;
}
