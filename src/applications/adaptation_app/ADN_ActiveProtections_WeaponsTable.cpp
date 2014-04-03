// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ActiveProtections_WeaponsTable.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_Resources_Data.h"
#include "protocol/Protocol.h"

//-----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable constructor
// Created: FDS 2010-02-24
//-----------------------------------------------------------------------------
ADN_ActiveProtections_WeaponsTable::ADN_ActiveProtections_WeaponsTable( const QString& strName, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << strName << tools::translate( "ADN_ActiveProtections_WeaponsTable", "Coefficient" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 1, 0.01 );
}

//-----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable destructor
// Created: FDS 2010-02-24
//-----------------------------------------------------------------------------
ADN_ActiveProtections_WeaponsTable::~ADN_ActiveProtections_WeaponsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_WeaponsTable::AddRow( int row, void* data )
{
    ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* pInfos = static_cast< ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &pInfos->coefficient_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable::OnContextMenu
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_WeaponsTable::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );

    ADN_Resources_Data::T_CategoryInfos_Vector& pWeapon = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( sword::dotation_type_ammunition ).categories_;
    for( auto it = pWeapon.begin(); it != pWeapon.end(); ++it )
    {
        if( Contains( **it ) )
            continue;
        addMenu.insertItem( ( *it )->strName_.GetData().c_str(), 2 + static_cast< int >( std::distance( pWeapon.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );

    menu.insertItem( tools::translate( "ADN_ActiveProtections_WeaponsTable", "New" ), &addMenu );
    menu.insertItem( tools::translate( "ADN_ActiveProtections_WeaponsTable", "Delete" ), 1 );
    menu.setItemEnabled( 1, GetSelectedData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        // Delete the current element.
        ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* pCurrent = static_cast< ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* >( GetSelectedData() );
        if( pCurrent != 0 )
            static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurrent );
    }
    else
    {
        // Create a new element
        ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* pNewInfo = new ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons();
        pNewInfo->SetCrossedElement( pWeapon[ nMenuResult - 2 ] );
        pNewInfo->coefficient_ = 0;

        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        pCTable.AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable::Contains
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
bool ADN_ActiveProtections_WeaponsTable::Contains( ADN_Resources_Data::CategoryInfo& category )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        const QModelIndex index = dataModel_.index( row, 1 );
        ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* pInfos = static_cast< ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* >( GetDataFromIndex( index ) );
        if( pInfos->GetCrossedElement() == &category )
            return true;
    }
    return false;
}
