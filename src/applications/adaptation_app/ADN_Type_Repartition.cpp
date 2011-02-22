// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Type_Repartition.h"

// -----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::ADN_Type_Repartition
// Created: MGD 2011-02-22
// -----------------------------------------------------------------------------
ADN_Type_Repartition::ADN_Type_Repartition()
: male_( 100 ),
  female_( 0 ),
  children_( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::~ADN_Type_Repartition
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
ADN_Type_Repartition::~ADN_Type_Repartition()
{
    //NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::operator =( const ADN_Type_Repartition& copy )
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
ADN_Type_Repartition& ADN_Type_Repartition::operator =( ADN_Type_Repartition& copy )
{
    male_ = copy.male_.GetData();
    female_ = copy.female_.GetData();
    children_ = copy.children_.GetData();
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::ReadArchive
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
void ADN_Type_Repartition::ReadArchive( xml::xistream& input )
{
    double rMale;
    double rFemale;
    double rChildren;

    input >> xml::attribute( "male", rMale )
          >> xml::attribute( "female", rFemale )
          >> xml::attribute( "children", rChildren );

    male_ = static_cast< int >( rMale * 100. );
    female_ = static_cast< int >( rFemale * 100. );
    children_ = static_cast< int >( rChildren * 100. );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::WriteArchive
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
void ADN_Type_Repartition::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "male", male_.GetData() / 100. )
           << xml::attribute( "female", female_.GetData() / 100. )
           << xml::attribute( "children", children_.GetData() / 100. );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::WriteArchive
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
bool ADN_Type_Repartition::CheckNoError()
{
    return male_.GetData() + female_.GetData() + children_.GetData() == 100;
}

