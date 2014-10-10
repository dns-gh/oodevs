// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AiEngine_Perception_Table.h"
#include "moc_ADN_AiEngine_Perception_Table.cpp"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_RadioButton.h"

ADN_AiEngine_Perception_Table::ADN_AiEngine_Perception_Table( const QString& objectName, ADN_Ref_ABC& vector, QWidget* parent /* = 0*/ )
    : QTableWidget( parent )
{
    setObjectName( objectName );
    setSortingEnabled( false );
    setSelectionMode( QAbstractItemView::NoSelection );
    setAlternatingRowColors( true );
    setShowGrid( false );
    verticalHeader()->setDefaultSectionSize( 20 );
    verticalHeader()->hide();
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    const auto labels = QStringList() << tr( "Information" ) << tr( "Detection" )
            << tr( "Recognition" )<< tr( "Identification" )<< tr( "Never" );
    setColumnCount( labels.size() );
    setHorizontalHeaderLabels( labels );
    setMinimumHeight( 250 );
    setMaximumHeight( 250 );
    pConnector_.reset( new ADN_Connector_Table_ABC( *this ) );
    pConnector_->Connect( &vector );
}

ADN_AiEngine_Perception_Table::~ADN_AiEngine_Perception_Table()
{
    // NOTHING
}

void ADN_AiEngine_Perception_Table::SetEnabled( bool enable )
{
    setEnabled( enable );
}

void ADN_AiEngine_Perception_Table::setNumRows( int rows )
{
    setRowCount( rows );
}

void ADN_AiEngine_Perception_Table::RemoveItem( void* /*item*/ )
{
    // NOTHING
}

void ADN_AiEngine_Perception_Table::AddRow( int row, void* data )
{
    static const QString perceptions[ 11 ] =
    {
        tr( "Heading" ),
        tr( "Speed" ),
        tr( "Operational State" ),
        tr( "Side" ),
        tr( "Level" ),
        tr( "Nature (partial)" ),
        tr( "Nature (full)" ),
        tr( "Surrendered" ),
        tr( "Prisoner" ),
        tr( "Refugees picked up" ),
        tr( "Command Post" )
    };
    if( row > boost::size( perceptions ) )
        throw MASA_EXCEPTION( "invalid row in ADN_AiEngine_Perception_Table" );
    auto infos = static_cast< ADN_AiEngine_Data::PerceptionInfos* >( data );
    if( !infos )
        return;
    setRowCount( std::max( rowCount(), row + 1 ) );
    setItem( row, 0, new QTableWidgetItem( perceptions[ row ] ) );
    QButtonGroup* group = new QButtonGroup( this );
    const auto addRadioButton =
        [&]( int column, ADN_Ref_ABC& target )
        {
            ADN_RadioButton* button = new ADN_RadioButton( this );
            group->addButton( button );
            button->GetConnector().Connect( &target );
            setCellWidget( row, column, button );
        };
    addRadioButton( 1, infos->detection_ );
    addRadioButton( 2, infos->recognition_ );
    addRadioButton( 3, infos->identification_ );
    addRadioButton( 4, infos->never_ );
}
