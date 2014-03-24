// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_ParameterValue.h"

ADN_Missions_ParameterValue::ADN_Missions_ParameterValue()
{
    // NOTHING
}

ADN_Missions_ParameterValue::~ADN_Missions_ParameterValue()
{
    // NOTHING
}

std::string ADN_Missions_ParameterValue::GetItemName()
{
    return name_.GetData();
}

ADN_Missions_ParameterValue* ADN_Missions_ParameterValue::CreateCopy()
{
    ADN_Missions_ParameterValue* newValue = new ADN_Missions_ParameterValue();
    newValue->name_ = name_.GetData();
    return newValue;
}

void ADN_Missions_ParameterValue::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ )
          >> xml::attribute( "id", id_ );
}

void ADN_Missions_ParameterValue::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "value" )
                << xml::attribute( "id", id_ )
                << xml::attribute( "name", name_ )
           << xml::end;
}
