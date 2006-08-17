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

#include <qpopmenu.h>

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Workspace.h"

typedef ADN_Composantes_Data::CategoryInfos CategoryInfos;


//-----------------------------------------------------------------------------
// Internal Table connector to be connected with munitions
//-----------------------------------------------------------------------------
class ADN_CT_Composantes_Dotations
: public ADN_Connector_Table_ABC
{
    MT_COPYNOTALLOWED( ADN_CT_Composantes_Dotations )

public:
    ADN_CT_Composantes_Dotations( ADN_Composantes_Dotations_GUI& tab)
    : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int n, void* pObj )
    {
        assert( pObj != 0 );
        CategoryInfos* pCategory = static_cast<CategoryInfos*>(pObj);

        // Add a new row.
        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, QTableItem::Never );
        ADN_TableItem_Double*    pItemQty  = new ADN_TableItem_Double( &tab_, pObj );
        ADN_TableItem_Double*    pItemLogThreshold  = new ADN_TableItem_Double( &tab_, pObj );
        ADN_TableItem_Double*    pItemNormalizedConsumption  = new ADN_TableItem_Double( &tab_, pObj );

        pItemQty->GetValidator().setRange( 0.001, INT_MAX, 3 );
        pItemLogThreshold->GetValidator().setRange( 0, 100, 2 );
        pItemNormalizedConsumption->GetValidator().setRange( 0.001, INT_MAX, 3 );

        tab_.setItem( n, 0, pItemName );
        tab_.setItem( n, 1, pItemQty );
        tab_.setItem( n, 2, pItemLogThreshold );
        tab_.setItem( n, 3, pItemNormalizedConsumption );

        // Connect the items
        pItemName->GetConnector().Connect( &pCategory->ptrCategory_.GetData()->strName_ );
        pItemQty->GetConnector().Connect( &pCategory->rNbr_ );
        pItemLogThreshold->GetConnector().Connect( &pCategory->rLogThreshold_ );
        pItemNormalizedConsumption->GetConnector().Connect( &pCategory->rNormalizedConsumption_ );
    }
};



//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Dotations_GUI::ADN_Composantes_Dotations_GUI( bool bIncludeNormalizedConsumption, QWidget* pParent )
: ADN_Table2( pParent, "ADN_Composantes_Dotations_GUI" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( QTable::NoSelection );
    setShowGrid( false );

    setMaximumHeight( 270 );

    // Hide the vertical header.
    verticalHeader()->hide();
    setLeftMargin( 0 );

    // Setup the columns.
    setNumCols( bIncludeNormalizedConsumption ? 4 : 3 );
    setNumRows( 0 );
//    setColumnStretchable( 0, true );
//    setColumnStretchable( 1, true );
//    setColumnStretchable( 2, false );
//    if( bIncludeNormalizedConsumption )
//        setColumnStretchable( 3, false );

    horizontalHeader()->setLabel( 0, tr( "Category" ) );
    horizontalHeader()->setLabel( 1, tr( "Qty" ) );
    horizontalHeader()->setLabel( 2, tr( "Log threshold (%)" ) );
    if( bIncludeNormalizedConsumption )
        horizontalHeader()->setLabel( 3, tr( "Normalized consumption" ) );

    // Connector creation
    pConnector_ = new ADN_CT_Composantes_Dotations( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Dotations_GUI::~ADN_Composantes_Dotations_GUI()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Dotations_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Composantes_Dotations_GUI::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
{
    QPopupMenu menu( this );
    QPopupMenu targetMenu( &menu );

    // Get the dotation list.
    ADN_Equipement_Data::T_DotationInfos_Vector& dotations 
        = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations(); 

    // Fill the popup menu with submenus, one for each dotation.
    for( ADN_Equipement_Data::IT_DotationInfos_Vector it = dotations.begin(); it != dotations.end(); ++it )
    {
        QPopupMenu* pSubMenu = new QPopupMenu( &targetMenu );

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

        targetMenu.insertItem( (*it)->strName_.GetData().c_str(), pSubMenu );
    }


    menu.insertItem( tr( "Add dotation"), &targetMenu ,0 );
    if( GetCurrentData() != 0 )
        menu.insertItem( tr( "Remove dotation" ), 1 );

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
    while( item( n, 1 ) != 0 )
    {
        ADN_TableItem_ABC* pItem = static_cast<ADN_TableItem_ABC*>( item( n, 1 ) );
        CategoryInfos* pCategory = static_cast<CategoryInfos*>( pItem->GetData() );
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
    CategoryInfos* pNewInfo = new CategoryInfos( category.parentDotation_ );
    pNewInfo->ptrCategory_ = &category;
    pNewInfo->rNbr_ = 0;

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
    assert( GetCurrentData() != 0 );
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( GetCurrentData() );
}