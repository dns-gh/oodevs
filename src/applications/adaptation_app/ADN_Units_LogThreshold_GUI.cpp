//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_LogThreshold_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 11 $
// $Workfile: ADN_Units_LogThreshold_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Units_LogThreshold_GUI.h"
#include "ADN_Categories_Data.h"
#include "ADN_Units_Data.h"
#include "ADN_LogisticSupplyClasses_Data.h"

//-----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_LogThreshold_GUI::ADN_Units_LogThreshold_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Units_LogThreshold_GUI", "Class of supply" )
                      << tools::translate( "ADN_Units_LogThreshold_GUI", "Log threshold (%)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Fixed );
    horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    setColumnWidth( 0, 250 );

    delegate_.AddDoubleSpinBoxOnColumn( 1, 0., 100. );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_LogThreshold_GUI::~ADN_Units_LogThreshold_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::AddRow
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::AddRow( int row, void* data )
{
    ADN_Units_Data::StockLogThresholdInfos* pInfo = static_cast< ADN_Units_Data::StockLogThresholdInfos* >( data );
    if( !pInfo )
        return;
    AddItem( row, 0, data, &pInfo->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfo->rLogThreshold_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );

    // Get the list.
    ADN_LogisticSupplyClasses_Data::T_LogisticSupplyClass_Vector& logisticSupplyClasses = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses ).GetLogisticSupplyClasses();

    // Fill the popup menu with submenus, one for each dotation.
    for( auto it = logisticSupplyClasses.begin(); it != logisticSupplyClasses.end(); ++it )
    {
        bool found = false;
        for( int i = 0; i < numRows() && !found; ++i )
        {
            QStandardItem* item = GetItem( i, 0 );
            if( item->text().toStdString() == (*it)->strName_.GetData() )
                found = true;
        }

        if( !found )
        {
            // This id is used to encode the category into the item.
            int nItemId = (int)(*it); // $$$$ ABR 2012-02-16: OMG
            targetMenu.insertItem( (*it)->strName_.GetData().c_str(), nItemId );
        }
    }
    if( targetMenu.count() > 0 )
        menu.insertItem( tools::translate( "ADN_Units_LogThreshold_GUI", "Add class"), &targetMenu ,0 );
    if( GetSelectedData() != 0 )
        menu.insertItem( tools::translate( "ADN_Units_LogThreshold_GUI", "Remove class" ), 1 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentLogSupplyClass();
    else if( nMenuResult > 1 )
        AddNewLogSupplyClass( *(ADN_LogisticSupplyClasses_Data::LogisticSupplyClass*)nMenuResult ); // $$$$ ABR 2012-02-16: OMFG
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::AddNewLogSupplyClass
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::AddNewLogSupplyClass( ADN_LogisticSupplyClasses_Data::LogisticSupplyClass& category )
{
    ADN_Units_Data::StockLogThresholdInfos* pNewInfo = new ADN_Units_Data::StockLogThresholdInfos();
    pNewInfo->SetCrossedElement( &category );
    pNewInfo->rLogThreshold_ = 10;

    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( pNewInfo );
    pCTable.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::RemoveCurrentLogSupplyClass
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::RemoveCurrentLogSupplyClass()
{
    ADN_Units_Data::StockLogThresholdInfos* param = static_cast< ADN_Units_Data::StockLogThresholdInfos* >( GetSelectedData() );
    if( param )
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( param );
}
