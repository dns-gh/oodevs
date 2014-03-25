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
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::ADN_Type_Repartition
// Created: MGD 2011-02-22
// -----------------------------------------------------------------------------
ADN_Type_Repartition::ADN_Type_Repartition( const QString& parentGuiName )
    : parentGuiName_( parentGuiName )
    , male_         ( 100. )
    , female_       ( 0. )
    , children_     ( 0. )
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

    male_ = rMale * 100.;
    female_ = rFemale * 100.;
    children_ = rChildren * 100.;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::WriteArchive
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
void ADN_Type_Repartition::WriteArchive( xml::xostream& output ) const
{
    output << xml::attribute( "male", male_.GetData() / 100. )
           << xml::attribute( "female", female_.GetData() / 100. )
           << xml::attribute( "children", children_.GetData() / 100. );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Repartition::WriteArchive
// Created: MGD 2011-02-22
//-----------------------------------------------------------------------------
void ADN_Type_Repartition::CheckNoError( const QString& dataName, ADN_ConsistencyChecker& checker, int tab ) const
{
    double total = male_.GetData() + female_.GetData() + children_.GetData();
    if( total!= 100 )
        checker.AddError( eRepartitionError, tools::translate( "ADN_Type_Repartition", "Invalid repartition on tab \'%1\' -"
                                                                        " Males/Females/Children repartition must be 100%."
                                                                        " Current value for \'%2\': %3%." )
                                                                        .arg( parentGuiName_ )
                                                                        .arg( dataName )
                                                                        .arg( total ).toStdString(), tab );
}
