// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionGenObjectTypes_Table.h"
#include "moc_ADN_MissionGenObjectTypes_Table.cpp"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TableItem_CheckItem.h"
#include "ADN_MissionGenObjectTypes_Infos.h"

class ADN_MissionGenObjectTypes_Table_Connector : public ADN_Connector_Table_ABC
{

public:
    explicit ADN_MissionGenObjectTypes_Table_Connector( ADN_MissionGenObjectTypes_Table& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void* obj )
    {
        assert( obj );
        ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, obj );
        ADN_TableItem_CheckItem* pItemAllowed = new ADN_TableItem_CheckItem ( &tab_, obj );

        // add a new row & set new values
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemAllowed );

        // connect items & datas
        pItemString->GetConnector().Connect( &static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( obj )->ptrObject_.GetData()->strName_ );
        pItemAllowed->GetConnector().Connect( &static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( obj )->isAllowed_ );
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Table::ADN_MissionGenObjectTypes_Table( QWidget* pParent, ADN_Connector_ABC*& pGuiConnector, QCheckBox* all )
    : ADN_Table( pParent, "ADN_MissionGenObjectTypes_Table" )
    , all_( all )
{
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setColumnReadOnly( 0, true );

    horizontalHeader()->setLabel( 0, tr( "Type" ) );
    horizontalHeader()->setLabel( 1, tr( "Allowed" ) );

    pConnector_ = new ADN_MissionGenObjectTypes_Table_Connector( *this );
    pGuiConnector = pConnector_;
    connect( all_, SIGNAL( stateChanged( int ) ), this, SLOT( OnStateChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table destructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Table::~ADN_MissionGenObjectTypes_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::IsChecked
// Created: LGY 2012-04-20
// -----------------------------------------------------------------------------
bool ADN_MissionGenObjectTypes_Table::IsChecked() const
{
    bool allChecked = true;
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        ADN_TableItem_CheckItem* pData = static_cast< ADN_TableItem_CheckItem* >( item( nRow, 1 ) );
        if( !static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( pData->GetData() )->isAllowed_.GetData() )
            return false;
    }
    return allChecked;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::doValueChanged
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::doValueChanged( int row, int col )
{
    ADN_Table::doValueChanged( row, col );
    all_->blockSignals( true );
    all_->setChecked( IsChecked() );
    all_->blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::OnTypeChanged
// Created: LGY 2012-04-20
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::OnTypeChanged( E_MissionParameterType type )
{
    if( type == eMissionParameterTypeGenObject )
    {
        all_->blockSignals( true );
        all_->setChecked( IsChecked() );
        all_->blockSignals( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::OnStateChanged
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::OnStateChanged( int state )
{
    if( numRows() > 0 && numCols() > 0)
        setCurrentCell( 0, 0 );
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        ADN_TableItem_CheckItem* pData = static_cast< ADN_TableItem_CheckItem* >( item( nRow, 1 ) );
        if( pData )
        {
            bool active = state == Qt::Checked;
            static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( pData->GetData() )->isAllowed_ = active;
            pData->setChecked( state == Qt::Checked );
        }
    }
    repaintContents( true );
}
