// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_SpeedEffect_Volume_ListView.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Crowds_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"

typedef ADN_Crowds_Data::SpeedEffectVolumeInfos SpeedEffectVolumeInfos;

class ADN_Sizes : public ADN_Connector_ListView_ABC
                , private boost::noncopyable
{

public:
    explicit ADN_Sizes( ADN_Crowds_SpeedEffect_Volume_ListView& list )
        : ADN_Connector_ListView_ABC( list )
    {
        // NOTHING
    }
    virtual ~ADN_Sizes()
    {
        // NOTHING
    }

    ADN_ListViewItem* CreateItem( void* pObj )
    {
        SpeedEffectVolumeInfos* infos = static_cast< SpeedEffectVolumeInfos* >( pObj );
        ADN_ListViewItem* pItem = new ADN_ListViewItem( pObj );
        if( infos && infos->GetCrossedElement() )
            pItem->Connect( &infos->GetCrossedElement()->strName_ );
        return pItem;
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_SpeedEffect_Volume_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_SpeedEffect_Volume_ListView::ADN_Crowds_SpeedEffect_Volume_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Crowds_SpeedEffect_Volume_ListView", tools::translate( "ADN_Crowds_SpeedEffect_Volume_ListView", "Volumes" ) )
{
    pConnector_.reset( new ADN_Sizes( *this ) );
    SetDeletionEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_SpeedEffect_Volume_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_SpeedEffect_Volume_ListView::~ADN_Crowds_SpeedEffect_Volume_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_SpeedEffect_Volume_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Crowds_SpeedEffect_Volume_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    SpeedEffectVolumeInfos* pInfos = (SpeedEffectVolumeInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Crowds_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Crowds_GUI::eSpeedEffectDensity] ->Connect( &pInfos->rDensity_ , bConnect );
    vItemConnectors_[ADN_Crowds_GUI::eSpeedEffectMaxSpeed]->Connect( &pInfos->rMaxSpeed_, bConnect );
}
