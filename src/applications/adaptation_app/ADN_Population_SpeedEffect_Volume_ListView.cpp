// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_SpeedEffect_Volume_ListView.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"

typedef ADN_Population_Data::SpeedEffectVolumeInfos SpeedEffectVolumeInfos;

class ADN_Sizes : public ADN_Connector_ListView_ABC
                , private boost::noncopyable
{

public:
    explicit ADN_Sizes( ADN_Population_SpeedEffect_Volume_ListView& list )
        : ADN_Connector_ListView_ABC( list, "ADN_Sizes" )
    {
        // NOTHING
    }
    virtual ~ADN_Sizes()
    {
        // NOTHING
    }

    ADN_ListViewItem* CreateItem( void* pObj )
    {
        ADN_ListViewItem* pItem = new ADN_ListViewItem( &list_, pObj, 1 );
        pItem->Connect( 0, static_cast< SpeedEffectVolumeInfos* >(pObj)->ptrVolume_.GetData() );
        return pItem;
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_Population_SpeedEffect_Volume_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_SpeedEffect_Volume_ListView::ADN_Population_SpeedEffect_Volume_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Volumes" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Sizes( *this );
    this->SetDeletionEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_SpeedEffect_Volume_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_SpeedEffect_Volume_ListView::~ADN_Population_SpeedEffect_Volume_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_SpeedEffect_Volume_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Population_SpeedEffect_Volume_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    SpeedEffectVolumeInfos* pInfos = (SpeedEffectVolumeInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Population_GUI::eSpeedEffectDensity] ->Connect( &pInfos->rDensity_ , bConnect );
    vItemConnectors_[ADN_Population_GUI::eSpeedEffectMaxSpeed]->Connect( &pInfos->rMaxSpeed_, bConnect );
}
