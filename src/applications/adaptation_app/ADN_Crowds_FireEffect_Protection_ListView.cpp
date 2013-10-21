// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_FireEffect_Protection_ListView.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Crowds_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"

typedef ADN_Crowds_Data::FireEffectProtectionInfos FireEffectProtectionInfos;

class ADN_Protections : public ADN_Connector_ListView_ABC
                      , private boost::noncopyable
{

public:
    ADN_Protections( ADN_Crowds_FireEffect_Protection_ListView& list )
        : ADN_Connector_ListView_ABC( list )
    {
        // NOTHING
    }
    virtual ~ADN_Protections()
    {
        // NOTHING
    }

    ADN_ListViewItem* CreateItem( void* pObj )
    {
        FireEffectProtectionInfos* infos = static_cast< FireEffectProtectionInfos* >( pObj );
        ADN_ListViewItem* pItem = new ADN_ListViewItem( pObj );
        if( infos && infos->GetCrossedElement() )
            pItem->Connect( &infos->GetCrossedElement()->strName_ );
        return pItem;
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_FireEffect_Protection_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_FireEffect_Protection_ListView::ADN_Crowds_FireEffect_Protection_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Crowds_FireEffect_Protection_ListView", tools::translate( "ADN_Crowds_FireEffect_Protection_ListView", "Protections" ) )
{
    pConnector_.reset( new ADN_Protections( *this ) );
    SetDeletionEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_FireEffect_Protection_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_FireEffect_Protection_ListView::~ADN_Crowds_FireEffect_Protection_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_FireEffect_Protection_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Crowds_FireEffect_Protection_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    FireEffectProtectionInfos* pInfos = (FireEffectProtectionInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Crowds_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Crowds_GUI::eFireEffectUnarmedDestruction]->Connect( &pInfos->rUnarmedDestruction_, bConnect );
    vItemConnectors_[ADN_Crowds_GUI::eFireEffectUnarmedFixableWithEvacuation]->Connect( &pInfos->rUnarmedFixableWithEvacuation_, bConnect );
    vItemConnectors_[ADN_Crowds_GUI::eFireEffectUnarmedFixableWithoutEvacuation]->Connect( &pInfos->rUnarmedFixableWithoutEvacuation_, bConnect );

    vItemConnectors_[ADN_Crowds_GUI::eFireEffectArmedDestruction]->Connect( &pInfos->rArmedDestruction_, bConnect );
    vItemConnectors_[ADN_Crowds_GUI::eFireEffectArmedFixableWithEvacuation]->Connect( &pInfos->rArmedFixableWithEvacuation_, bConnect );
    vItemConnectors_[ADN_Crowds_GUI::eFireEffectArmedFixableWithoutEvacuation]->Connect( &pInfos->rArmedFixableWithoutEvacuation_, bConnect );
}
