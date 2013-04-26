// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_DescriptionParameter.h"
#include "ADN_Missions_Parameter.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Missions_GUI.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionParameter constructor
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
ADN_ListView_DescriptionParameter::ADN_ListView_DescriptionParameter()
    : ADN_ListView( 0, "ADN_ListView_DescriptionParameter", tr( "Parameters" ) )
{
    pConnector_ = new ADN_Connector_ListView< ADN_Missions_Parameter >( *this );
    setHeaderHidden( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionParameter destructor
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
ADN_ListView_DescriptionParameter::~ADN_ListView_DescriptionParameter()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionParameter::ConnectItem
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionParameter::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ADN_Missions_Parameter* pInfos = ( ADN_Missions_Parameter* )pCurData_;
    vItemConnectors_[ ADN_Missions_GUI::eDescriptionParametersText ]->Connect( &pInfos->description_, bConnect );
}