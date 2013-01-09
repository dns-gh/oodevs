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
#include "ADN_MissionGenObjectTypes_Infos.h"
#include "ADN_TableItem_CheckItem.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Connector_Bool.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_ModificatorIlluminationInfos_Vector
//-----------------------------------------------------------------------------
class ADN_MissionGenObjectTypes_Table_Connector : public ADN_Connector_Table_ABC
{
public:
    ADN_MissionGenObjectTypes_Table_Connector( ADN_MissionGenObjectTypes_Table& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void* obj )
    {
        helpers::ADN_MissionGenObjectTypes_Infos* param = static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( obj );

        ADN_TableItem_String* itemName = new ADN_TableItem_String( &tab_, obj, Q3TableItem::Never );
        ADN_TableItem_CheckItem* itemAllowed = new ADN_TableItem_CheckItem ( &tab_, obj );

        tab_.setItem( i, 0, itemName );
        tab_.setItem( i, 1, itemAllowed );

        itemName->setText( param->ptrObject_.GetData()->strName_.GetData().c_str() );
        itemName->GetConnector().Connect( &param->ptrObject_.GetData()->strName_ );
        itemAllowed->GetConnector().Connect( &param->isAllowed_ );
    }

private:
    ADN_MissionGenObjectTypes_Table_Connector& operator=( const ADN_MissionGenObjectTypes_Table_Connector& );
};

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Table::ADN_MissionGenObjectTypes_Table( QCheckBox* all, const QString& objectName, QWidget* pParent /*= 0*/ )
    : ADN_Table2( pParent, objectName )
    , all_( all )
{
    setObjectName( objectName );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin(0);
    verticalHeader()->hide();

    setNumCols(2);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);
    horizontalHeader()->setLabel(0, tr( "Type" ) );
    horizontalHeader()->setLabel(1, tr( "Allowed" ) );

    connect( all_, SIGNAL( stateChanged( int ) ), this, SLOT( OnStateChanged( int ) ) );

    pConnector_ = new ADN_MissionGenObjectTypes_Table_Connector( *this );
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
// Name: ADN_MissionGenObjectTypes_Table::FinalizeConnection
// Created: ABR 2013-01-09
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::FinalizeConnection()
{
    setSorting( true );
    sortColumn( 0, true, true );
    setSorting( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::IsChecked
// Created: LGY 2012-04-20
// -----------------------------------------------------------------------------
bool ADN_MissionGenObjectTypes_Table::IsChecked() const
{
    // Check if everything is checked
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        ADN_TableItem_CheckItem* item = static_cast< ADN_TableItem_CheckItem* >( this->item( nRow, 1 ) );
        if( item && !item->IsChecked() )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::OnTypeChanged
// Created: LGY 2012-04-20
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::OnTypeChanged( E_MissionParameterType type )
{
    if( type == eMissionParameterTypeObjectKnowledge )
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
    clearSelection();
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        ADN_TableItem_CheckItem* item = static_cast< ADN_TableItem_CheckItem* >( this->item( nRow, 1 ) );
        if( item )
            static_cast< ADN_Connector_Bool< ADN_TableItem_CheckItem >& >( item->GetConnector() ).SetDataChanged( state == Qt::Checked );
    }
    repaintContents( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::doValueChanged
// Created: ABR 2013-01-08
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::doValueChanged( int row, int col )
{
    ADN_Table2::doValueChanged( row, col );
    all_->blockSignals( true );
    all_->setChecked( IsChecked() );
    all_->blockSignals( false );
}
