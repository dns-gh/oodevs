// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Models_MissionInfos.h"
#include "ADN_Missions_Data.h"
#include "ADN_Models_OrderInfos.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_Models_MissionInfos constructor
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_MissionInfos::ADN_Models_MissionInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission /* = 0 */ )
    : ADN_CrossedRef< ADN_Missions_ABC >( missions, mission, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_MissionInfos destructor
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_MissionInfos::~ADN_Models_MissionInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_MissionInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_MissionInfos* ADN_Models_MissionInfos::CreateCopy()
{
    ADN_Models_MissionInfos* pMission = new ADN_Models_MissionInfos( GetVector(), GetCrossedElement() );

    pMission->vOrders_.reserve( vOrders_.size() );
    for( auto it = vOrders_.begin(); it != vOrders_.end(); ++it )
        pMission->vOrders_.AddItem( ( *it )->CreateCopy() );
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_MissionInfos::ReadFragOrder
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_MissionInfos::ReadFragOrder( xml::xistream& input )
{
    std::auto_ptr< ADN_Models_OrderInfos > spNew( new ADN_Models_OrderInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ) ) );
    spNew->ReadArchive( input );
    vOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_MissionInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_MissionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
    ADN_Missions_ABC* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( GetVector(), strName_.GetData() );
    if( !mission )
        throw MASA_EXCEPTION( tools::translate( "ADN_Models_MissionInfos", "Doctrine models - Invalid mission '%1'" ).arg( strName_.GetData().c_str() ).toStdString() );
    SetCrossedElement( mission );
    input >> xml::list( "fragorder", *this, &ADN_Models_MissionInfos::ReadFragOrder );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_MissionInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_MissionInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "mission" )
        << xml::attribute( "name", *this );
    for( auto it = vOrders_.begin(); it != vOrders_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}
