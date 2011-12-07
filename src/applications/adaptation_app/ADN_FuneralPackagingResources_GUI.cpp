//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_FuneralPackagingResources_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 11 $
// $Workfile: ADN_FuneralPackagingResources_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_FuneralPackagingResources_GUI.h"

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Workspace.h"
#include "ADN_FuneralPackagingResource.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Tr.h"
#include "ADN_TableItem_TimeField.h"
#include "ADN_TableItem_CheckItem.h"
#include "ENT/Ent_Tr.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_FuneralPackagingResources_GUI_Connector
:public ADN_Connector_Table_ABC
{
public:

    ADN_FuneralPackagingResources_GUI_Connector( ADN_FuneralPackagingResources_GUI& tab )
    : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems( int nRow, void* pObj )
    {
        assert( pObj != 0 );
        ADN_FuneralPackagingResource* pInfo = static_cast< ADN_FuneralPackagingResource* >( pObj );

        // Add a new row.
        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_TimeField* pItemProcessDuration = new ADN_TableItem_TimeField( &tab_, pObj );
        ADN_TableItem_CheckItem* pItemTerminal = new ADN_TableItem_CheckItem( &tab_, pObj );

        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemProcessDuration );
        tab_.setItem( nRow, 2, pItemTerminal );

        pItemName->GetConnector().Connect( &pInfo->resource_.GetData()->strName_ );
        pItemProcessDuration->GetConnector().Connect( &pInfo->processDuration_ );
        pItemTerminal->GetConnector().Connect( &pInfo->terminal_ );
    }

private:
    ADN_FuneralPackagingResources_GUI_Connector& operator=( const ADN_FuneralPackagingResources_GUI_Connector& );
};

//-----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_FuneralPackagingResources_GUI::ADN_FuneralPackagingResources_GUI( QWidget* pParent )
: ADN_Table2( pParent, "ADN_FuneralPackagingResources_GUI" )
{
    setMinimumHeight( 200 );
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    // setShowGrid( false );

    // Hide the vertical header.
    verticalHeader()->hide();
    setLeftMargin( 0 );

    // Setup the columns.
    setNumRows( 0 );
    setNumCols( 3 );

    horizontalHeader()->setLabel( 0, tr( "Resource" ) );
    horizontalHeader()->setLabel( 1, tr( "Packaging duration" ) );
    horizontalHeader()->setLabel( 2, tr( "Is terminal" ) );

    setColumnWidth( 0, 200 );

    pConnector_ = new ADN_FuneralPackagingResources_GUI_Connector( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_FuneralPackagingResources_GUI::~ADN_FuneralPackagingResources_GUI()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
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
    if( GetCurrentData() != 0 )
        menu.insertItem( tr( "Remove resource" ), 1 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentDotation();
    else if( nMenuResult > 1 )
        AddNewDotation( *(ADN_Equipement_Data::CategoryInfo*)nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_FuneralPackagingResources_GUI::Contains( ADN_Equipement_Data::CategoryInfo& category )
{
    int n = 0;
    while( item( n, 1 ) != 0 )
    {
        ADN_TableItem_ABC* pItem = static_cast<ADN_TableItem_ABC*>( item( n, 1 ) );
        ADN_FuneralPackagingResource* pCategory = static_cast<ADN_FuneralPackagingResource*>( pItem->GetData() );
        if( pCategory->resource_ == &category )
            return true;
        ++n;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::AddNewDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::AddNewDotation( ADN_Equipement_Data::CategoryInfo& category )
{
    ADN_FuneralPackagingResource* pNewInfo = new ADN_FuneralPackagingResource();
    pNewInfo->resource_ = &category;
    pNewInfo->processDuration_ = "1h";
    pNewInfo->terminal_ = false;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::RemoveCurrentDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::RemoveCurrentDotation()
{
    assert( GetCurrentData() != 0 );
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( GetCurrentData() );
}