// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Template_Usages.h"
#include "moc_ADN_Template_Usages.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages constructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Template_Usages::ADN_Template_Usages( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Usage" ) << tr( "Proportion(%)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddSpinBoxOnColumn( 1 );
    delegate_.SetSingleColumnSumRestriction( 1, gui::CommonDelegate::eLTE, 100 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages destructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Template_Usages::~ADN_Template_Usages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages::OnContextMenu
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Template_Usages::OnContextMenu( const QPoint& point )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );
    ADN_Urban_Data::T_AccommodationInfos_Vector& accommodations = ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos();
    for( auto it = accommodations.begin(); it != accommodations.end(); ++it )
    {
        if( Contains( **it ) )
            continue;
        addMenu.insertItem( (*it)->strName_.GetData().c_str(), 2 + static_cast< int >( std::distance( accommodations.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );
    menu.insertItem( tr( "New" ), &addMenu );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetSelectedData() != 0 );
    int nMenuResult = menu.exec( point );
    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        ADN_Urban_Data::UsageTemplateInfos* pCurrent = (ADN_Urban_Data::UsageTemplateInfos*)GetSelectedData();
        if( pCurrent != 0 )
            static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurrent );
    }
    else
    {
        ADN_Urban_Data::AccommodationInfos* pResult = accommodations[ nMenuResult - 2 ];
        ADN_Urban_Data::UsageTemplateInfos* pNewInfo = new ADN_Urban_Data::UsageTemplateInfos( *pResult, 0 );

        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        pCTable.AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages::Contains
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
bool ADN_Template_Usages::Contains( ADN_Urban_Data::AccommodationInfos& accommodation )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        const QModelIndex index = dataModel_.index( row, 1 );
        ADN_Urban_Data::UsageTemplateInfos* pInfos = static_cast< ADN_Urban_Data::UsageTemplateInfos* >( GetDataFromIndex( index ) );
        if( pInfos && pInfos->GetCrossedElement() && pInfos->GetCrossedElement()->strName_.GetData() == accommodation.strName_.GetData() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages::AddRow
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Template_Usages::AddRow( int row, void* data )
{
    ADN_Urban_Data::UsageTemplateInfos* pInfos = static_cast< ADN_Urban_Data::UsageTemplateInfos* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->proportion_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
