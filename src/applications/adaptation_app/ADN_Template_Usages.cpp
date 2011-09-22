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
#include "ADN_Connector_Table_ABC.h"

class ADN_CT_Template_Usages : public ADN_Connector_Table_ABC
{
public:
    ADN_CT_Template_Usages( ADN_Template_Usages& table )
        : ADN_Connector_Table_ABC( table, false )
        , table_( table )
    {
        // NOTHING
    }

    void  AddSubItems( int nRow, void* pObj )
    {
        assert( pObj != 0 );

        ADN_Urban_Data::UsageTemplateInfos* pInfos = static_cast< ADN_Urban_Data::UsageTemplateInfos* >( pObj );

        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_Int* pItemValue = new ADN_TableItem_Int( &tab_, pObj );
        pItemValue->GetValidator().setRange( 0, 100 );
        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemValue );
        pItemName->setEnabled( false );
        pItemName->GetConnector().Connect( &pInfos->strName_ );
        pItemValue->GetConnector().Connect( &pInfos->proportion_ );
    }

private:
    ADN_Template_Usages& table_;

    ADN_CT_Template_Usages& operator=( const ADN_CT_Template_Usages& );
};


// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages constructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Template_Usages::ADN_Template_Usages( QWidget* parent )
    : ADN_Table2( parent, "ADN_Template_Usages" )
{
    setSorting( true );
    setSelectionMode( Q3Table::Single );
    setShowGrid( false );
    setFixedHeight( 150 );
    verticalHeader()->hide();
    setLeftMargin( 0 );
    setNumCols( 2 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "Usage" ) );
    horizontalHeader()->setLabel( 1, tr( "Proportion(%)" ) );
    pConnector_ = new ADN_CT_Template_Usages( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages destructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Template_Usages::~ADN_Template_Usages()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages::OnContextMenu
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Template_Usages::OnContextMenu( int /*row*/, int /*col*/, const QPoint& point )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );
    ADN_Urban_Data::T_AccommodationInfos_Vector& accommodations = ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos();
    for( ADN_Urban_Data::IT_AccommodationInfos_Vector it = accommodations.begin(); it != accommodations.end(); ++it )
    {
        if( Contains( **it ) )
            continue;
        addMenu.insertItem( (*it)->strName_.GetData().c_str(), (int)(*it) );
    }
    ADN_Tools::SortMenu( addMenu );
    menu.insertItem( tr( "New" ), &addMenu );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetCurrentData() != 0 );
    int nMenuResult = menu.exec( point );
    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        ADN_Urban_Data::UsageTemplateInfos* pCurrent = (ADN_Urban_Data::UsageTemplateInfos*)GetCurrentData();
        if( pCurrent != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
    }
    else
    {
        ADN_Urban_Data::AccommodationInfos* pResult = (ADN_Urban_Data::AccommodationInfos*)nMenuResult;
        ADN_Urban_Data::UsageTemplateInfos* pNewInfo = new ADN_Urban_Data::UsageTemplateInfos();
        pNewInfo->proportion_ = 0;
        pNewInfo->strName_ = pResult->strName_.GetData();
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Template_Usages::Contains
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
bool ADN_Template_Usages::Contains( ADN_Urban_Data::AccommodationInfos& accommodation )
{
    int n = 0;
    while( item( n, 1 ) != 0 )
    {
        ADN_TableItem_ABC* pItem = static_cast< ADN_TableItem_ABC* >( item( n, 1 ) );
        ADN_Urban_Data::UsageTemplateInfos* pInfos = static_cast< ADN_Urban_Data::UsageTemplateInfos* >( pItem->GetData() );
        if( pInfos->strName_.GetData() == accommodation.strName_.GetData() )
            return true;
        ++n;
    }
    return false;
}
