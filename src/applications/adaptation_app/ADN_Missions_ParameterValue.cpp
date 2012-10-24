// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_ParameterValue.h"

#include "tools/Loader_ABC.h"
#include <tools/XmlCrc32Signature.h>
#include <xeuseuleu/xsl.hpp>

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
    input >> xml::attribute( "name", name_ );
}

void ADN_Missions_ParameterValue::WriteArchive( xml::xostream& output, unsigned int id )
{
    output << xml::start( "value" )
                << xml::attribute( "id", id )
                << xml::attribute( "name", name_ )
           << xml::end;
}
