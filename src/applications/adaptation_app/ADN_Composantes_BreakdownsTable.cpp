// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Composantes_BreakdownsTable.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 10:36 $
// $Revision: 2 $
// $Workfile: ADN_Composantes_BreakdownsTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composantes_BreakdownsTable.h"
#include "moc_ADN_Composantes_BreakdownsTable.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Composantes_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Workspace.h"

typedef ADN_Composantes_Data::BreakdownInfos BreakdownInfos;


//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_Composantes_BreakdownsTable
//-----------------------------------------------------------------------------
class ADN_CT_Composantes_BreakdownsTable
    : public ADN_Connector_Table_ABC
{
public:

    ADN_CT_Composantes_BreakdownsTable( ADN_Composantes_BreakdownsTable& table )
        : ADN_Connector_Table_ABC( table, false )
        , breakdownsTable_       ( table )
    {}

    void  AddSubItems( int nRow, void* pObj )
    {
        assert( pObj != 0 );
        BreakdownInfos* pBreakdown = static_cast<BreakdownInfos*>( pObj );

        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_Double* pItemOdds = new ADN_TableItem_Double( &tab_, pObj );

        // add a new row & set new values
        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemOdds );

        // set table item properties
        pItemName->setEnabled( false );
        pItemName->GetConnector().Connect( &pBreakdown->ptrBreakdown_.GetData()->strName_ );

        // set table item properties
        pItemOdds->GetValidator().setRange( 0.0, 100.0, 2 );
        pItemOdds->GetConnector().Connect( &pBreakdown->rPercentage_ );

        breakdownsTable_.OnModified();
    }

private:
    ADN_Composantes_BreakdownsTable& breakdownsTable_;

    ADN_CT_Composantes_BreakdownsTable& operator=( const ADN_CT_Composantes_BreakdownsTable& );
};



//-----------------------------------------------------------------------------
// Name: ADN_Composantes_BreakdownsTable constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_BreakdownsTable::ADN_Composantes_BreakdownsTable( const std::string& strName, QWidget* pParent )
:   ADN_Table2( pParent, "ADN_Composantes_BreakdownsTable" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::Single );
    setShowGrid( false );

    setMinimumHeight( 150 );
    setMaximumHeight( 150 );

    // hide vertical header
    verticalHeader()->hide();
    setLeftMargin( 0 );

    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );
//    setColumnStretchable( 0, true );
//    setColumnStretchable( 1, false );

    horizontalHeader()->setLabel( 0, strName.c_str() );
    horizontalHeader()->setLabel( 1, tr( "Odds (%)" ) );

    // connector creation
    pConnector_ = new ADN_CT_Composantes_BreakdownsTable(*this);

    connect( this, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnModified() ) );
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_BreakdownsTable destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_BreakdownsTable::~ADN_Composantes_BreakdownsTable()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_BreakdownsTable::OnContextMenu
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_BreakdownsTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );

    ADN_Breakdowns_Data& breakdownsData = ADN_Workspace::GetWorkspace().GetBreakdowns().GetData();
    for( ADN_Breakdowns_Data::IT_BreakdownInfoVector it = breakdownsData.vBreakdowns_.begin(); it != breakdownsData.vBreakdowns_.end(); ++it )
        addMenu.insertItem( (*it)->strName_.GetData().c_str(), (int)(*it) );

    menu.insertItem( tr( "New" ), &addMenu );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetCurrentData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        // Delete the current element.
        BreakdownInfos* pCurrent = (BreakdownInfos*)GetCurrentData();
        if( pCurrent != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
    }
    else
    {
        // Create a new element
        ADN_Breakdowns_Data::BreakdownInfo* pCast = (ADN_Breakdowns_Data::BreakdownInfo*)nMenuResult;
        BreakdownInfos* pNewInfo = new BreakdownInfos();
        pNewInfo->ptrBreakdown_ = pCast;
        pNewInfo->ptrBreakdown_.SetVector( ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().vBreakdowns_ );
        //pNewInfo->ptrBreakdown_.SetVector( pCast->nti_.vBreakdowns_ );
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_BreakdownsTable::OnModified
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_BreakdownsTable::OnModified()
{
    double rSum = 0;
    for( int n = 0; n < numRows(); ++n )
    {
        bool bOk = false;
        double r = text( n, 1 ).toDouble( &bOk );
        if( bOk )
            rSum += r;
    }

    if( rSum == 100.0 )
    {
        QColorGroup cg = this->horizontalHeader()->colorGroup();
        cg.setColor( QColorGroup::ButtonText, Qt::black );
        QPalette palette( this->horizontalHeader()->palette() );
        palette.setActive( cg );
        palette.setDisabled( cg );
        palette.setInactive( cg );
        this->horizontalHeader()->setPalette( palette );
    }
    else
    {
        QColorGroup cg = this->horizontalHeader()->colorGroup();
        cg.setColor( QColorGroup::ButtonText, Qt::red );
        QPalette palette( this->horizontalHeader()->palette() );
        palette.setActive( cg );
        palette.setDisabled( cg );
        palette.setInactive( cg );
        this->horizontalHeader()->setPalette( palette );
    }
}
