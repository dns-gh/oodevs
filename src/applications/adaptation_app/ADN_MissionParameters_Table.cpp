// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameters_Table.h"
#include "moc_ADN_MissionParameters_Table.cpp"
#include "ADN_Missions_Data.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::ADN_MissionParameters_Table
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
ADN_MissionParameters_Table::ADN_MissionParameters_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 6 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Name" )
                      << tr( "DirectIA name" )
                      << tr( "Type" )
                      << tr( "Optional" )
                      << tr( "Range min" )
                      << tr( "Range max" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 2, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 3, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 4, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 5, QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );

    for( unsigned int i = 0; i < unsigned int( eNbrMissionParameterType ); ++i )
        parameterTypes_ << ADN_Tr::ConvertFromMissionParameterType( static_cast< E_MissionParameterType >( i ), ENT_Tr_ABC::eToTr ).c_str();

    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddLineEditOnColumn( 1 );
    delegate_.AddComboBoxOnColumn( 2, parameterTypes_ );
    delegate_.AddCheckBoxOnColumn( 3 );
    delegate_.AddSpinBoxOnColumn( 4, 0, std::numeric_limits< int >::max() );
    delegate_.AddSpinBoxOnColumn( 5, 0, std::numeric_limits< int >::max() );

    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged( const QItemSelection&, const QItemSelection& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::~ADN_MissionParameters_Table
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
ADN_MissionParameters_Table::~ADN_MissionParameters_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::AddRow
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::AddRow( int row, void* data )
{
    ADN_Missions_Parameter* pMission = static_cast< ADN_Missions_Parameter* >( data );
    if( !pMission )
        return;

    addingRow_ = true;
    AddItem( row, 0, data, &pMission->strName_, ADN_StandardItem::eString, Qt::ItemIsEditable );
    AddItem( row, 1, data, &pMission->diaName_, ADN_StandardItem::eString, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pMission->type_, parameterTypes_, Qt::ItemIsEditable );
    AddItem( row, 3, data, &pMission->isOptional_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 4, data, &pMission->minOccurs_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 5, data, &pMission->maxOccurs_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    addingRow_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::SetItemConnectors
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::SetItemConnectors( const T_ConnectorVector& itemConnectors )
{
    itemConnectors_ = itemConnectors;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::OnTypeChanged
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::OnTypeChanged( E_MissionParameterType type )
{
    emit TypeChanged( type );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::OnContextMenu
// Created: ABR 2012-10-29
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popup( this );

    popup.insertItem( tr( "Add parameter"), 0 );
    if( GetSelectedData() != 0 )
        popup.insertItem( tr( "Remove parameter"), 1 );

    int result = popup.exec( pt );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::AddNewElement
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::AddNewElement()
{
    ADN_Missions_Parameter* newElement = new ADN_Missions_Parameter();
    newElement->strName_ = tr( "New parameter" ).toStdString();

    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    connector->AddItem( newElement );
    connector->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::RemoveCurrentElement
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::RemoveCurrentElement()
{
    ADN_Missions_Parameter* param = static_cast< ADN_Missions_Parameter* >( GetSelectedData() );
    if( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
    ResetCurrent();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::ResetCurrent
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::ResetCurrent()
{
    emit TypeChanged( eNbrMissionParameterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::dataChanged
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    if( !addingRow_ && topLeft == bottomRight && topLeft.column() == 2 )
        Reconnect( topLeft );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::OnSelectionChanged
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::OnSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    if( deselected.indexes().size() == 1 &&
        ( selected.indexes().size() == 0 || ( selected.indexes().size() == 1 && selected.indexes()[ 0 ] != deselected.indexes()[ 0 ] ) ) )
        Disconnect( static_cast< ADN_Missions_Parameter* >( GetDataFromIndex( deselected.indexes()[ 0 ] ) ) );
    if( selected.indexes().size() == 1 )
        Reconnect( selected.indexes()[ 0 ] );
    else if( selected.indexes().size() == 0 )
        ResetCurrent();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::Reconnect
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::Reconnect( const QModelIndex& index )
{
    ADN_Missions_Parameter* param = static_cast< ADN_Missions_Parameter* >( GetDataFromIndex( index ) );
    if( !param )
        return;
    Disconnect( param );
    E_MissionParameterType current = param->type_.GetData();
    if( current == eMissionParameterTypeEnumeration )
        itemConnectors_[ADN_Missions_GUI::eParameterValues]->Connect( &param->values_ );
    else
    {
        param->values_.Clear();
        if( current == eMissionParameterTypeLocationComposite )
            itemConnectors_[ADN_Missions_GUI::eChoiceValues]->Connect( &param->choices_ );
        else if( current == eMissionParameterTypeNumeric )
        {
            itemConnectors_[ADN_Missions_GUI::eMinValue]->Connect( &param->minValue_ );
            itemConnectors_[ADN_Missions_GUI::eMaxValue]->Connect( &param->maxValue_ );
        }
        else if( current == eMissionParameterTypeGenObject )
            itemConnectors_[ADN_Missions_GUI::eGenObjects]->Connect( &param->genObjects_ );
    }
    emit TypeChanged( current );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::Disconnect
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::Disconnect( ADN_Missions_Parameter* param )
{
    if( param )
    {
        itemConnectors_[ ADN_Missions_GUI::eParameterValues ]->Disconnect( &param->values_ );
        itemConnectors_[ ADN_Missions_GUI::eChoiceValues ]->Disconnect( &param->choices_ );
        itemConnectors_[ ADN_Missions_GUI::eMinValue ]->Disconnect( &param->minValue_ );
        itemConnectors_[ ADN_Missions_GUI::eMaxValue ]->Disconnect( &param->maxValue_ );
        itemConnectors_[ ADN_Missions_GUI::eGenObjects ]->Disconnect( &param->genObjects_ );
    }
}
