// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_EquipementSelector.h"
#include "moc_ADN_EquipementSelector.cpp"
#include "ADN_Workspace.h"
#include "ADN_Equipement_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector::ADN_EquipementSelector
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_EquipementSelector::ADN_EquipementSelector( QWidget* pParent, const char* szName )
    : QPushButton( pParent, szName )
    , ADN_Gfx_ABC()
    , pConnected_( 0 )
    , pData_     ( 0 )
{
    setText( tr( "Click here to select" ) );
    pConnector_ = new ADN_EquipementSelector_Connector( *this );
    connect( this, SIGNAL( clicked() ), this, SLOT( OnButtonPressed() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector destructor
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_EquipementSelector::~ADN_EquipementSelector()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector::OnButtonPressed
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
void ADN_EquipementSelector::OnButtonPressed()
{
    Q3PopupMenu* pPopup = new Q3PopupMenu( this );

    // Get the dotation list.
    ADN_Equipement_Data::T_ResourceInfos_Vector& dotations
        = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations();

    // Fill the popup menu with submenus, one for each dotation.
    for( ADN_Equipement_Data::IT_ResourceInfos_Vector it = dotations.begin(); it != dotations.end(); ++it )
    {
        Q3PopupMenu* pSubMenu = new Q3PopupMenu( pPopup );

        // Fill the submenu with an entry for each equipement category.
        ADN_Equipement_Data::T_CategoryInfos_Vector& categories = (*it)->GetCategories();
        for( ADN_Equipement_Data::IT_CategoryInfos_Vector it2 = categories.begin(); it2 != categories.end(); ++it2 )
        {
            // This id is used to encode the category into the item.
            int nItemId = (int)(*it2);
            pSubMenu->insertItem( (*it2)->strName_.GetData().c_str(), nItemId );
        }

        pPopup->insertItem( (*it)->strName_.GetData().c_str(), pSubMenu );
    }

    int nResult = pPopup->exec( QCursor::pos() );

    // If an item was selected...
    if( nResult != -1 )
    {
        // Reteive the category from the result, and use the connector to notify its selection.
        this->SetItem( (void*)nResult );
        static_cast<ADN_EquipementSelector_Connector*>(pConnector_)->NotifySelected( (void*)nResult );
    }

    delete pPopup;
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector::ItemRemoved
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
void ADN_EquipementSelector::ItemRemoved( void* pItem )
{
    if( pData_ != pItem )
        return;

    disconnect( pConnected_, 0, this, SLOT( ItemRemoved( void* ) ) );
    pConnected_ = 0;
    static_cast<ADN_EquipementSelector_Connector*>(pConnector_)->NotifySelected( (void*)0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector::SetItem
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
void ADN_EquipementSelector::SetItem( void* pItem )
{
    if( pConnected_ != 0 )
        disconnect( pConnected_, 0, this, SLOT( ItemRemoved( void* ) ) );

    pConnected_ = 0;
    pData_ = 0;

    if( pItem == 0 )
    {
        this->setText( tr( "Click here to select" ) );
        return;
    }

    pConnected_ = & ((ADN_Equipement_Data::CategoryInfo*)pItem)->parentResource_.categories_;
    pData_ = pItem;
    connect( pConnected_, SIGNAL( ItemRemoved( void* ) ), this, SLOT( ItemRemoved( void* ) ) );
    setText( ((ADN_Equipement_Data::CategoryInfo*)pItem)->strName_.GetData().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector_Connector::ADN_EquipementSelector_Connector
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_EquipementSelector::ADN_EquipementSelector_Connector::ADN_EquipementSelector_Connector( ADN_EquipementSelector& gui )
    : ADN_Connector_Vector_ABC()
    , gui_( gui )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector_Connector::~ADN_EquipementSelector_Connector
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_EquipementSelector::ADN_EquipementSelector_Connector::~ADN_EquipementSelector_Connector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector_Connector::NotifySelected
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
void ADN_EquipementSelector::ADN_EquipementSelector_Connector::NotifySelected( void* pData )
{
    emit DataChanged( pData );
}

// -----------------------------------------------------------------------------
// Name: ADN_EquipementSelector_Connector::SetDataPrivate
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
void ADN_EquipementSelector::ADN_EquipementSelector_Connector::SetDataPrivate( void* pData )
{
    if( pData == 0 )
        gui_.SetItem( 0 );
    else
        gui_.SetItem( pData );
}
