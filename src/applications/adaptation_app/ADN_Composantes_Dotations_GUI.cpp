//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Dotations_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 11 $
// $Workfile: ADN_Composantes_Dotations_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "moc_ADN_Composantes_Dotations_GUI.cpp"
#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Composantes_Data::CategoryInfos CategoryInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Dotations_GUI::ADN_Composantes_Dotations_GUI( const QString& objectName, ADN_Connector_ABC*& connector,
                                                              bool bIncludeNormalizedConsumption, QWidget* pParent, bool bIncludeThreshold )
    : ADN_Table3( objectName, connector, pParent )
    , bIncludeNormalizedConsumption_( bIncludeNormalizedConsumption )
    , bIncludeThreshold_( bIncludeThreshold )
{
    // Setup the columns.
    const int cols = 2 + static_cast< int >( bIncludeNormalizedConsumption_ ) + static_cast< int >( bIncludeThreshold );
    dataModel_.setColumnCount( cols );

    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Category" )
                      << tr( "Qty" );
    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddSpinBoxOnColumn( 1, 1, INT_MAX );
    if( bIncludeThreshold )
    {
        horizontalHeaders << tr( "Log threshold (%)" );
        delegate_.AddDoubleSpinBoxOnColumn( 2, 0.0, 100.0, 0.01, 2 );
    }
    if( bIncludeNormalizedConsumption )
    {
        horizontalHeaders << tr( "Normalized consumption" );
        delegate_.AddDoubleSpinBoxOnColumn( 2 + static_cast< int >( bIncludeThreshold ), 0.001, INT_MAX, 0.001, 3 );
    }
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );
    setMaximumHeight( 270 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Dotations_GUI::~ADN_Composantes_Dotations_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Composantes_Dotations_GUI::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );

    // Get the dotation list.
    ADN_Equipement_Data::T_ResourceInfos_Vector& dotations
        = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations();

    // Fill the popup menu with submenus, one for each dotation.
    for( ADN_Equipement_Data::IT_ResourceInfos_Vector it = dotations.begin(); it != dotations.end(); ++it )
    {
        Q3PopupMenu* pSubMenu = new Q3PopupMenu( &targetMenu );

        // Fill the submenu with an entry for each equipement category.
        ADN_Equipement_Data::T_CategoryInfos_Vector& categories = (*it)->GetCategories();
        for( ADN_Equipement_Data::IT_CategoryInfos_Vector it2 = categories.begin(); it2 != categories.end(); ++it2 )
        {
            // Avoid offering to add items already present.
            if( Contains( **it2 ) )
                continue;

            // This id is used to encode the category into the item.
            int nItemId = (int)(*it2);
            pSubMenu->insertItem( (*it2)->strName_.GetData().c_str(), nItemId );
        }
        ADN_Tools::SortMenu( *pSubMenu );

        targetMenu.insertItem( ENT_Tr::ConvertFromDotationFamily( (*it)->nType_, ENT_Tr_ABC::eToTr ).c_str(), pSubMenu );
    }

    menu.insertItem( tr( "Add resource"), &targetMenu ,0 );
    if( GetSelectedData() != 0 )
        menu.insertItem( tr( "Remove resource" ), 1 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentDotation();
    else if( nMenuResult > 1 )
        AddNewDotation( *(ADN_Equipement_Data::CategoryInfo*)nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Composantes_Dotations_GUI::Contains( ADN_Equipement_Data::CategoryInfo& category )
{
    int n = 0;
    while( dataModel_.item( n, 1 ) != 0 )
    {
        ADN_StandardItem* pItem = static_cast< ADN_StandardItem* >( dataModel_.item( n, 1 ) );
        CategoryInfos* pCategory = static_cast< CategoryInfos* >( pItem->GetData() );
        if( pCategory->ptrCategory_.GetData() == &category )
            return true;
        ++n;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI::AddNewDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Composantes_Dotations_GUI::AddNewDotation( ADN_Equipement_Data::CategoryInfo& category )
{
    CategoryInfos* pNewInfo = new CategoryInfos( category.parentResource_ );
    pNewInfo->ptrCategory_ = &category;
    pNewInfo->rNbr_ = 1;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI::RemoveCurrentDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Composantes_Dotations_GUI::RemoveCurrentDotation()
{
    assert( GetSelectedData() != 0 );
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( GetSelectedData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Composantes_Dotations_GUI::AddRow( int row, void* data )
{
    CategoryInfos* pCategory = static_cast< CategoryInfos* >( data );
    if( !pCategory )
        return;

    AddItem( row, 0, data, &pCategory->ptrCategory_.GetData()->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &pCategory->rNbr_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    if( bIncludeThreshold_ )
        AddItem( row, 2, data, &pCategory->rLogThreshold_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    if( bIncludeNormalizedConsumption_ )
        AddItem( row, 2 + static_cast< int >( bIncludeThreshold_ ), data, &pCategory->rNormalizedConsumption_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
