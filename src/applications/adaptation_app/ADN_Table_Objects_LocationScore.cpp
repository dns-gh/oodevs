// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Table_Objects_LocationScore.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 12 $
// $Workfile: ADN_Table_Objects_LocationScore.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Table_Objects_LocationScore.h"
#include "moc_ADN_Table_Objects_LocationScore.cpp"
#include "ADN_Objects_Data.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Objects_Data::ScoreLocationInfos ScoreLocationInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore constructor
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Table_Objects_LocationScore::ADN_Table_Objects_LocationScore( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Ground Type" )
                      << tr( "Weight" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 1 );
    setMinimumHeight( 205 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore destructor
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Table_Objects_LocationScore::~ADN_Table_Objects_LocationScore()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::OnContextMenu
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Table_Objects_LocationScore::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( &popupMenu );

    // Get the list of the possible munitions
    for( int i = 0; i < eNbrLocation; ++i )
    {
        std::string strTerrainName = ENT_Tr::ConvertFromLocation( (E_Location)i, ENT_Tr_ABC::eToTr );
        if( Contains( strTerrainName ) )
            continue;

        addMenu.insertItem( strTerrainName.c_str(), 2 + i );
    }

    ADN_Tools::SortMenu( addMenu );
    popupMenu.insertItem( tr( "Add terrain" ), &addMenu , 0 );
    if( GetData( pt ) != 0 )
        popupMenu.insertItem( tr( "Remove terrain" ), 1 );

    int nResult = popupMenu.exec( pt );
    if( nResult == 1 )
        RemoveCurrentElement();
    else if( nResult > 1 )
    {
        assert( nResult - 2 < eNbrLocation );
        AddNewElement( nResult - 2 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::AddNewElement
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Table_Objects_LocationScore::AddNewElement( int n )
{
    ScoreLocationInfos* newElement = new ScoreLocationInfos();
    if( newElement )
        newElement->nLocation_ = (E_Location)n;
    ADN_Connector_Vector_ABC& connector = static_cast< ADN_Connector_Vector_ABC& >( GetConnector() );
    connector.AddItem( newElement );
    connector.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::Contains
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
bool ADN_Table_Objects_LocationScore::Contains( const std::string& strMunitionName )
{
    int i = 0;
    while( ADN_StandardItem* pItem =  static_cast< ADN_StandardItem* >( dataModel_.item( i, 1 ) ) )
    {
        ScoreLocationInfos* pInfos = static_cast< ScoreLocationInfos* >( pItem->GetData() );
        if( pInfos->nLocation_.Convert( ENT_Tr_ABC::eToTr ) == strMunitionName )
            return true;
        ++i;
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Table_Objects_LocationScore::AddRow( int row, void* data )
{
    ScoreLocationInfos* pCurComposante = static_cast< ScoreLocationInfos* >( data );
    if( !pCurComposante )
        return;
    AddItem( row, 0, data, QString::fromStdString( pCurComposante->nLocation_.Convert( ENT_Tr_ABC::eToTr ) ), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pCurComposante->nScore_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
