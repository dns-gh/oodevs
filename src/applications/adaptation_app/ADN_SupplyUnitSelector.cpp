// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SupplyUnitSelector.h"
#include "moc_ADN_SupplyUnitSelector.cpp"
#include "ADN_Workspace.h"
#include "ADN_Units_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector::ADN_SupplyUnitSelector
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_SupplyUnitSelector::ADN_SupplyUnitSelector( QWidget* pParent, const char* szName )
    : QPushButton( pParent, szName )
    , ADN_Gfx_ABC()
    , pData_     ( 0 )
{
    setText( tr( "Click here to select" ) );
    pConnector_ = new ADN_SupplyUnitSelector_Connector( *this );
    connect( this, SIGNAL( clicked() ), this, SLOT( OnButtonPressed() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector destructor
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_SupplyUnitSelector::~ADN_SupplyUnitSelector()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector::OnButtonPressed
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
void ADN_SupplyUnitSelector::OnButtonPressed()
{
    Q3PopupMenu* pPopup = new Q3PopupMenu( this );

    // Get the unit list.
    ADN_Units_Data::T_UnitInfos_Vector& units
        = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();

    // Fill the popup menu with items, one for each log unit.
    for( ADN_Units_Data::IT_UnitInfos_Vector it = units.begin(); it != units.end(); ++it )
    {
        if( (*it)->eTypeId_ != eAgentTypePionLOGConvoi )
            continue;

        // This id is used to encode the unit into the item.
        int nItemId = (int)(*it);
        pPopup->insertItem( (*it)->strName_.GetData().c_str(), nItemId );
    }

    int nResult = pPopup->exec( QCursor::pos() );

    // If an item was selected...
    if( nResult != -1 )
    {
        // Reteive the unit from the result, and use the connector to notify its selection.
        this->SetItem( (void*)nResult );
        static_cast<ADN_SupplyUnitSelector_Connector*>(pConnector_)->NotifySelected( (void*)nResult );
    }

    delete pPopup;
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector::ItemRemoved
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
void ADN_SupplyUnitSelector::ItemRemoved( void* pItem )
{
    if( pData_ != pItem )
        return;

    // Get the unit list.
    ADN_Units_Data::T_UnitInfos_Vector& units
        = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();

    disconnect( &units, 0, this, SLOT( ItemRemoved( void* ) ) );
    static_cast<ADN_SupplyUnitSelector_Connector*>(pConnector_)->NotifySelected( (void*)0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector::SetItem
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
void ADN_SupplyUnitSelector::SetItem( void* pItem )
{
    // Get the unit list.
    ADN_Units_Data::T_UnitInfos_Vector& units
        = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();

    disconnect( &units, 0, this, SLOT( ItemRemoved( void* ) ) );

    pData_ = 0;

    if( pItem == 0 )
    {
        this->setText( tr( "Click here to select" ) );
        return;
    }

    pData_ = pItem;
    connect( &units, SIGNAL( ItemRemoved( void* ) ), this, SLOT( ItemRemoved( void* ) ) );
    setText( ((ADN_Units_Data::UnitInfos*)pItem)->strName_.GetData().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector_Connector::ADN_SupplyUnitSelector_Connector
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_SupplyUnitSelector::ADN_SupplyUnitSelector_Connector::ADN_SupplyUnitSelector_Connector( ADN_SupplyUnitSelector& gui )
: ADN_Connector_Vector_ABC()
, gui_( gui )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector_Connector::~ADN_SupplyUnitSelector_Connector
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_SupplyUnitSelector::ADN_SupplyUnitSelector_Connector::~ADN_SupplyUnitSelector_Connector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector_Connector::NotifySelected
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
void ADN_SupplyUnitSelector::ADN_SupplyUnitSelector_Connector::NotifySelected( void* pData )
{
    emit DataChanged( pData );
}

// -----------------------------------------------------------------------------
// Name: ADN_SupplyUnitSelector_Connector::SetDataPrivate
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
void ADN_SupplyUnitSelector::ADN_SupplyUnitSelector_Connector::SetDataPrivate( void* pData )
{
    if( pData == 0 )
        gui_.SetItem( 0 );
    else
        gui_.SetItem( pData );
}
