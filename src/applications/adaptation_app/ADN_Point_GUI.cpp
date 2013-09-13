//*****************************************************************************
//
// $Created: AGN 03-07-30 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Point_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 9 $
// $Workfile: ADN_Point_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Point_GUI.h"
#include "moc_ADN_Point_GUI.cpp"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Units_Data.h"
#include "ADN_Tr.h"

typedef ADN_Units_Data::PointInfos PointInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Point_GUI constructor
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
ADN_Point_GUI::ADN_Point_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setMaximumHeight( 300 );

    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Ground Type" )
                      << tr( "Distance (m)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    delegate_.AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Point_GUI destructor
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
ADN_Point_GUI::~ADN_Point_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::AddRow
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Point_GUI::AddRow( int row, void* data )
{
    PointInfos* point = static_cast< PointInfos* >( data );
    if( !data )
        return;

    AddItem( row, 0, data, ADN_Tr::ConvertFromKeyPoint( point->nTypeTerrain_, ADN_Tr::eToApp ).c_str(), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &point->nDistance_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

//-----------------------------------------------------------------------------
// Name: ADN_Point_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Point_GUI::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu subMenu( &menu );

    menu.insertItem( tr( "Add point" ), &subMenu );
    if( GetSelectedData() != 0 )
        menu.insertItem( tr( "Remove point" ), 100 );

    for( int n = 0; n < eNbrKeyPoints; ++n )
    {
        if( !Contains( (E_KeyPoint)n ) )
            subMenu.insertItem( ADN_Tr::ConvertFromKeyPoint( (E_KeyPoint)n ).c_str(), n );
    }

    int nMenuResult = menu.exec( pt );
    if( nMenuResult == 100 )
        DeleteCurrentElement();
    else if( nMenuResult >= 0 )
        AddNewElement( (E_KeyPoint)nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::AddNewElement
// Created: AGN 2003-12-08
// -----------------------------------------------------------------------------
void ADN_Point_GUI::AddNewElement( E_KeyPoint nKeyPoint )
{
    assert( nKeyPoint >= 0 && nKeyPoint < eNbrKeyPoints );
    PointInfos* pNewInfo = new PointInfos();
    pNewInfo->nTypeTerrain_ = nKeyPoint;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::DeleteCurrentElement
// Created: AGN 2003-12-08
// -----------------------------------------------------------------------------
void ADN_Point_GUI::DeleteCurrentElement()
{
    PointInfos* pCurComposante = static_cast< PointInfos* >( GetSelectedData() );
    if( pCurComposante )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurComposante );
}

// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Point_GUI::Contains( E_KeyPoint nKeyPoint )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        PointInfos* infos = static_cast< PointInfos* >( GetData( row, 1 ) );
        if( infos->nTypeTerrain_ == nKeyPoint )
            return true;
    }
    return false;
}
