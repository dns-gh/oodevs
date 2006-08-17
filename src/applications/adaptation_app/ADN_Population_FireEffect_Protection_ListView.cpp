// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Population_FireEffect_Protection_ListView.h"

#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"

typedef ADN_Population_Data::FireEffectProtectionInfos FireEffectProtectionInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Protection_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_FireEffect_Protection_ListView::ADN_Population_FireEffect_Protection_ListView( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Protections" ) );
    setSortColumn( -1 );
    setResizeMode( QListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView< FireEffectProtectionInfos >( *this );

    this->SetDeletionEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Protection_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_FireEffect_Protection_ListView::~ADN_Population_FireEffect_Protection_ListView()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Protection_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Population_FireEffect_Protection_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    FireEffectProtectionInfos* pInfos = (FireEffectProtectionInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Population_GUI::eFireEffectDestruction]             ->Connect( &pInfos->rDestruction_             , bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectFixableWithEvacuation]   ->Connect( &pInfos->rFixableWithEvacuation_   , bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectFixableWithoutEvacuation]->Connect( &pInfos->rFixableWithoutEvacuation_, bConnect );
}

