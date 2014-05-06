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
#include "moc_ADN_FuneralPackagingResources_GUI.cpp"

#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Workspace.h"
#include "ADN_FuneralPackagingResource.h"
#include "ADN_Resources_Data.h"
#include "ADN_Tr.h"
#include "ENT/Ent_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_FuneralPackagingResources_GUI::ADN_FuneralPackagingResources_GUI( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent /* = 0 */ )
: ADN_Table( objectName, vector, pParent )
{
    setMinimumHeight( 200 );
    dataModel_.setColumnCount( 3 );
    setSortingEnabled( true );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Supplies" )
                      << tr( "Packaging duration" )
                      << tr( "Is terminal" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDelayEditOnColumn( 1 );
    delegate_.AddCheckBoxOnColumn( 2 );
    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).AddItem( 0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_FuneralPackagingResources_GUI::~ADN_FuneralPackagingResources_GUI()
{
    //NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );

    // Get the dotation list.
    ADN_Resources_Data::T_ResourceInfos_Vector& dotations
        = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources();

    // Fill the popup menu with submenus, one for each dotation.
    for( auto it = dotations.begin(); it != dotations.end(); ++it )
    {
        Q3PopupMenu* pSubMenu = new Q3PopupMenu( &targetMenu );

        // Fill the submenu with an entry for each equipement category.
        ADN_Resources_Data::T_CategoryInfos_Vector& categories = (*it)->GetCategories();
        for( auto it2 = categories.begin(); it2 != categories.end(); ++it2 )
        {
            // Avoid offering to add items already present.
            if( Contains( **it2 ) )
                continue;

            // This id is used to encode the category into the item.
            int nItemId = (int)(*it2);
            pSubMenu->insertItem( (*it2)->strName_.GetData().c_str(), nItemId );
        }
        ADN_Tools::SortMenu( *pSubMenu );

        targetMenu.insertItem( ENT_Tr::ConvertFromDotationType( (*it)->nType_, ENT_Tr::eToTr ).c_str(), pSubMenu );
    }

    menu.insertItem( tr( "Add supplies"), &targetMenu ,0 );
    if( GetSelectedData() != 0 )
        menu.insertItem( tr( "Remove supplies" ), 1 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentDotation();
    else if( nMenuResult > 1 )
        AddNewDotation( *(ADN_Resources_Data::CategoryInfo*)nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_FuneralPackagingResources_GUI::Contains( ADN_Resources_Data::CategoryInfo& category )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        ADN_StandardItem* pItem = static_cast< ADN_StandardItem*>( dataModel_.item( row, 1 ) );
        if( pItem )
        {
            ADN_FuneralPackagingResource* pCategory = static_cast<ADN_FuneralPackagingResource*>( pItem->GetData() );
            if( pCategory && pCategory->GetCrossedElement() == &category )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::AddNewDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::AddNewDotation( ADN_Resources_Data::CategoryInfo& category )
{
    ADN_FuneralPackagingResource* pNewInfo = new ADN_FuneralPackagingResource();
    pNewInfo->SetCrossedElement( &category );
    pNewInfo->SetVector( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( category.parentResource_.nType_ ).GetCategories() );
    pNewInfo->processDuration_ = "1h";
    pNewInfo->terminal_ = false;

    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( pNewInfo );
    pCTable.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::AddRow
// Created: NPT 2012-11-06
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::AddRow( int row, void* data )
{
    ADN_FuneralPackagingResource* pInfo = static_cast< ADN_FuneralPackagingResource* >( data );
    if( !pInfo )
        return;
    AddItem( row, 0, data, &pInfo->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfo->processDuration_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pInfo->terminal_, ADN_StandardItem::eBool, Qt::ItemIsUserCheckable );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResources_GUI::RemoveCurrentDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResources_GUI::RemoveCurrentDotation()
{
    assert( GetSelectedData() != 0 );
    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( GetSelectedData() );
    emit CurrentRemoved();
}
