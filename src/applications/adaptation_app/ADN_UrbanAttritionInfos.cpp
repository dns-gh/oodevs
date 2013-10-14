// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_UrbanAttritionInfos.h"
#include "ADN_WorkspaceElement.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ADN_UrbanAttritionInfos::ADN_UrbanAttritionInfos
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
ADN_UrbanAttritionInfos::ADN_UrbanAttritionInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), ptr, true )
    , rCoeff_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UrbanAttritionInfos::CopyFrom
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
void ADN_UrbanAttritionInfos::CopyFrom( ADN_UrbanAttritionInfos& attritions )
{
    rCoeff_ = attritions.rCoeff_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_UrbanAttritionInfos::ReadArchive
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
void ADN_UrbanAttritionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_UrbanAttritionInfos::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_UrbanAttritionInfos::WriteArchive( xml::xostream& output, const std::string& tag /* = "urbanModifier"*/ )
{
    output << xml::start( tag )
             << xml::attribute( "material-type", *this )
             << xml::attribute( "value", rCoeff_.GetData() )
           << xml::end;
}
