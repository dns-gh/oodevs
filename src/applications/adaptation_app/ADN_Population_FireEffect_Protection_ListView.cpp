// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_FireEffect_Protection_ListView.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"

typedef ADN_Population_Data::FireEffectProtectionInfos FireEffectProtectionInfos;

class ADN_Protections : public ADN_Connector_ListView_ABC
                      , private boost::noncopyable
{

public:
    ADN_Protections( ADN_Population_FireEffect_Protection_ListView& list )
        : ADN_Connector_ListView_ABC( list, "ADN_Protections" )
    {
        // NOTHING
    }
    virtual ~ADN_Protections()
    {
        // NOTHING
    }

    ADN_ListViewItem* CreateItem( void* pObj )
    {
        ADN_ListViewItem* pItem = new ADN_ListViewItem( &list_, pObj, 1 );
        pItem->Connect( 0, &static_cast< FireEffectProtectionInfos* >(pObj)->ptrProtection_.GetData()->strName_ );
        return pItem;
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Protection_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_FireEffect_Protection_ListView::ADN_Population_FireEffect_Protection_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Protections" ) );
    setSortColumn( -1 );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Protections( *this );
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

    ADN_Workspace::GetWorkspace().GetPopulation().GetGui().UpdatePopulationFireEffectValidator(
        pInfos->rUnarmedDestruction_, pInfos->rUnarmedFixableWithEvacuation_, pInfos->rUnarmedFixableWithoutEvacuation_,
        pInfos->rArmedDestruction_, pInfos->rArmedFixableWithEvacuation_, pInfos->rArmedFixableWithoutEvacuation_ );

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Population_GUI::eFireEffectUnarmedDestruction]->Connect( &pInfos->rUnarmedDestruction_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectUnarmedFixableWithEvacuation]->Connect( &pInfos->rUnarmedFixableWithEvacuation_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectUnarmedFixableWithoutEvacuation]->Connect( &pInfos->rUnarmedFixableWithoutEvacuation_, bConnect );

    vItemConnectors_[ADN_Population_GUI::eFireEffectArmedDestruction]->Connect( &pInfos->rArmedDestruction_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectArmedFixableWithEvacuation]->Connect( &pInfos->rArmedFixableWithEvacuation_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectArmedFixableWithoutEvacuation]->Connect( &pInfos->rArmedFixableWithoutEvacuation_, bConnect );
}
