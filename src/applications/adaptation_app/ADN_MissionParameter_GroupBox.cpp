// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameter_GroupBox.h"
#include "moc_ADN_MissionParameter_GroupBox.cpp"
#include "ADN_StandardItem.h"
#include "ADN_Missions_Type.h"
#include <boost/assign.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionParameter_GroupBox::ADN_MissionParameter_GroupBox( int strips, Qt::Orientation orientation,
                                                              const QString& title, E_MissionParameterType authorized )
    : Q3GroupBox( strips, orientation, title )
    , authorized_( 1, authorized )
    , typeVisible_( false )
    , filterMissionType_( eNbrMissionParameterType )
    , isFilterMissionType_( false )
    , objectVisible_( true )
{
    hide();
}

ADN_MissionParameter_GroupBox::ADN_MissionParameter_GroupBox( int strips, Qt::Orientation orientation, const QString& title,
                                                              const std::vector< E_MissionParameterType >& authorized, E_MissionParameterType filterMissionType )
    : Q3GroupBox( strips, orientation, title )
    , authorized_( authorized )
    , filterMissionType_( filterMissionType )
    , isFilterMissionType_( false )
    , typeVisible_( false )
    , objectVisible_( true )
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox destructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionParameter_GroupBox::~ADN_MissionParameter_GroupBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox::OnTypeChanged
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
void ADN_MissionParameter_GroupBox::OnTypeChanged( E_MissionParameterType type )
{
    typeVisible_ = std::find( authorized_.begin(), authorized_.end(), type ) != authorized_.end();
    isFilterMissionType_ = type == filterMissionType_;
    setVisible( typeVisible_ && ( !isFilterMissionType_ || objectVisible_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox::OnMissionTypeChanged
// Created: JSR 2014-11-27
// -----------------------------------------------------------------------------
void ADN_MissionParameter_GroupBox::OnMissionTypeChanged( const QStandardItem& item )
{
    const ADN_StandardItem& adnItem = static_cast< const ADN_StandardItem& >( item );
    assert( adnItem.GetType() == ADN_StandardItem::eBool );
    ADN_Missions_Type* pInfos = static_cast< ADN_Missions_Type* >( adnItem.GetData() );
    if( pInfos->GetItemName() == "ObjectKnowledge" )
    {
        objectVisible_ = pInfos->isAllowed_.GetData();
        setVisible( typeVisible_ && ( !isFilterMissionType_ || objectVisible_ ) );
    }
}
