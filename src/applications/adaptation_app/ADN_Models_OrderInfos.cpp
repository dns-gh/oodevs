// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Models_OrderInfos.h"
#include "ADN_Missions_Data.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_Models_OrderInfos constructor
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
ADN_Models_OrderInfos::ADN_Models_OrderInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission /* = 0 */ )
    : ADN_CrossedRef< ADN_Missions_ABC >( missions, mission, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_OrderInfos constructor
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
ADN_Models_OrderInfos::ADN_Models_OrderInfos( ADN_Missions_FragOrder* fragorder, const std::string& name )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ), fragorder, true )
{
    strName_ = name;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_OrderInfos destructor
// Created: ABR 2014-04-10
// -----------------------------------------------------------------------------
ADN_Models_OrderInfos::~ADN_Models_OrderInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_OrderInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_OrderInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_OrderInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_OrderInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "fragorder" )
             << xml::attribute( "name", *this )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_OrderInfos::CreateCopy
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
ADN_Models_OrderInfos* ADN_Models_OrderInfos::CreateCopy()
{
    return new ADN_Models_OrderInfos( GetVector(), GetCrossedElement() );
}
