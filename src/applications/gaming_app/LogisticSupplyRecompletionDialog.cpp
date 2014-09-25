// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyRecompletionDialog.h"
#include "moc_LogisticSupplyRecompletionDialog.cpp"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "gaming/Dotations.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "gaming/Troops.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/LogisticHelpers.h"
#include "ENT/ENT_Tr.h"
#include "protocol/Simulation.h"

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog constructor
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
LogisticSupplyRecompletionDialog::LogisticSupplyRecompletionDialog( QWidget* parent, Controllers& controllers, const ::StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( parent, tr( "Recompletion" ) )
    , controllers_( controllers )
    , static_( staticModel )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Recompletion" ) );
    resize( 300, 500 );
    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this );
    mainLayout->setSpacing( 5 );
    mainLayout->setMargin( 5 );

    // Equipment and personal
    Q3GroupBox* equiPersoGroupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Equipment - Crew" ), this );
    equiPersoGroupBox->setFlat( true );

    equipmentsTable_ = new QTableWidget( 0, 3, equiPersoGroupBox );
    equipmentDelegate_ = new gui::CommonDelegate( this );
    equipmentsTable_->setItemDelegate( equipmentDelegate_ );
    equipmentsTable_->setColumnCount( 3 );
    QStringList headers;
    headers << tr( "Equipment" ) << tr( "Count" ) << tr( "Max" );
    equipmentsTable_->setHorizontalHeaderLabels( headers );
    equipmentsTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    equipmentsTable_->setColumnWidth( 1, 60 );
    equipmentsTable_->setColumnWidth( 2, 60 );
    equipmentsTable_->horizontalHeader()->setEditTriggers( 0 );
    equipmentsTable_->verticalHeader()->setVisible( false );
    equipmentsTable_->setContentsMargins( QMargins( 0, 0, 0, 0 ) );
    equipmentsTable_->setShowGrid( false );

    personalsTable_ = new QTableWidget( 0, 3, equiPersoGroupBox );
    personalDelegate_ = new gui::CommonDelegate( this );
    personalsTable_->setItemDelegate( personalDelegate_ );
    headers.clear();
    headers << tr( "Crew" ) << tr( "Count" ) << tr( "Max" );
    personalsTable_->setHorizontalHeaderLabels( headers );
    personalsTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    personalsTable_->setColumnWidth( 1, 60 );
    personalsTable_->setColumnWidth( 2, 60 );
    personalsTable_->horizontalHeader()->setEditTriggers( 0 );
    personalsTable_->verticalHeader()->setVisible( false );
    personalsTable_->setContentsMargins( QMargins( 0, 0, 0, 0 ) );
    personalsTable_->setShowGrid( false );

    mainLayout->addWidget( equiPersoGroupBox );

    // Dotations && !munition
    Q3GroupBox* dotationsGroupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Basic load" ), this );
    dotationsGroupBox->setFlat( true );

    dotationsTable_ = new QTableWidget( 0, 2, dotationsGroupBox );
    dotationDelegate_ = new gui::CommonDelegate( this );
    dotationsTable_->setItemDelegate( dotationDelegate_ );
    headers.clear();
    headers << tr( "Supplies" ) << tr( "Quantity %" );
    dotationsTable_->setHorizontalHeaderLabels( headers );
    dotationsTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    dotationsTable_->setColumnWidth( 0, 60 );
    dotationsTable_->horizontalHeader()->setEditTriggers( 0 );
    dotationsTable_->verticalHeader()->setVisible( false );
    dotationsTable_->setContentsMargins( QMargins( 0, 0, 0, 0 ) );
    dotationsTable_->setShowGrid( false );

    // Munitions Families
    munitionsFamilyTable_ = new QTableWidget( 0, 2, dotationsGroupBox );
    munitionsFamilyTable_->setColumnCount( 2 );
    munitionsFamilyDelegate_ = new gui::CommonDelegate( this );
    munitionsFamilyTable_->setItemDelegate( munitionsFamilyDelegate_ );
    headers.clear();
    headers << tr( "Ammo" ) << tr( "Quantity %" );
    munitionsFamilyTable_->setHorizontalHeaderLabels( headers );
    munitionsFamilyTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    munitionsFamilyTable_->setColumnWidth( 0, 60 );
    munitionsFamilyTable_->horizontalHeader()->setEditTriggers( 0 );
    munitionsFamilyTable_->verticalHeader()->setVisible( false );
    munitionsFamilyTable_->setContentsMargins( QMargins( 0, 0, 0, 0 ) );
    munitionsFamilyTable_->setShowGrid( false );
    mainLayout->addWidget( dotationsGroupBox );

    // Stocks
    stockGroupBox_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Stocks" ), this );
    stockGroupBox_->setFlat( true );

    stockTable_ = new QTableWidget( 0, 3, stockGroupBox_ );
    stockTable_->setColumnCount( 3 );
    stockDelegate_ = new gui::CommonDelegate( this );
    stockTable_->setItemDelegate( stockDelegate_ );
    headers.clear();
    headers << tr( "Supplies" ) << tr( "Count" ) << tr( "Norm. quantities" );
    stockTable_->setHorizontalHeaderLabels( headers );
    stockTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    stockTable_->setColumnWidth( 0, 60 );
    stockTable_->horizontalHeader()->setEditTriggers( 0 );
    stockTable_->verticalHeader()->setVisible( false );
    stockTable_->setContentsMargins( QMargins( 0, 0, 0, 0 ) );
    stockTable_->setShowGrid( false );

    mainLayout->addWidget( stockGroupBox_ );

    // ok / cancel butons
    Q3HBoxLayout* buttonLayout = new Q3HBoxLayout( mainLayout );
    QPushButton* okButton     = new QPushButton( tr("Ok")    , this );
    QPushButton* cancelButton = new QPushButton( tr("Cancel"), this );
    buttonLayout->addWidget( okButton     );
    buttonLayout->addWidget( cancelButton );
    okButton->setDefault( TRUE );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    connect( dotationsTable_      , SIGNAL( cellChanged( int, int ) ), SLOT( OnDotationChanged      ( int, int ) ) );
    connect( munitionsFamilyTable_, SIGNAL( cellChanged( int, int ) ), SLOT( OnMunitionFamilyChanged( int, int ) ) );
    connect( equipmentsTable_     , SIGNAL( cellChanged( int, int ) ), SLOT( OnEquipmentChanged     ( int, int ) ) );
    connect( personalsTable_      , SIGNAL( cellChanged( int, int ) ), SLOT( OnPersonalChanged      ( int, int ) ) );
    connect( stockTable_          , SIGNAL( cellChanged( int, int ) ), SLOT( OnStockChanged         ( int, int ) ) );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::closeEvent
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::closeEvent( QCloseEvent * )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog destructor
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
LogisticSupplyRecompletionDialog::~LogisticSupplyRecompletionDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeEquipments
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeEquipments()
{
    equipmentsList_.clear();
    equipmentsList_.append( "" );
    equipmentsMax_.clear();
    equipments_.clear();
    if( auto equipments = static_cast< const Equipments* >( selected_->Retrieve< Equipments_ABC >() ) )
    {
        auto it = equipments->CreateIterator();
        QStringList equipmentList;
        while( it.HasMoreElements() )
        {
            const Equipment& equipment = it.NextElement();
            equipmentsList_.append( equipment.GetName() );
            equipments_[ equipment.GetName() ] = &equipment;
            equipmentsMax_.push_back( equipment.Total() );
        }
        equipmentsTable_->setRowCount( 0 );
        equipmentsTable_->insertRow( 0 );
        equipmentDelegate_->AddComboBox( 0, 0, 0, 0, equipmentsList_ );
        equipmentsTable_->setItem( 0, 2,  new QTableWidgetItem() );
        equipmentsTable_->setMinimumHeight( std::max< int >( equipmentsTable_->rowHeight( 0 ) * 3, 20 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializePersonal
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializePersonal()
{
    if( const Troops* troops = selected_->Retrieve< Troops >() )
    {
        personalsTable_->setRowCount( 0 );
        AddPersonal( sword::officer, tr( "officers" ), troops->GetTotalByRank( eHumanRank_Officier ) );
        AddPersonal( sword::sub_officer, tr( "warrant-officers" ), troops->GetTotalByRank( eHumanRank_SousOfficer ) );
        AddPersonal( sword::trooper, tr( "private" ), troops->GetTotalByRank( eHumanRank_Mdr ) );
        personalsTable_->setMinimumHeight( std::max< int >( personalsTable_->rowHeight( 0 ) * 4, 20 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::AddPersonal
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::AddPersonal( unsigned nPos, const QString& label, unsigned nMax )
{
    personalsTable_->insertRow( nPos );
    QTableWidgetItem* item = new QTableWidgetItem( label );
    item->setCheckState( Qt::Unchecked );
    personalsTable_->setItem( nPos, 0, item );
    personalDelegate_->AddSpinBox( nPos, nPos, 1, 1, 0, nMax );
    personalsTable_->setItem( nPos, 1, new QTableWidgetItem( locale().toString( 0 ) ) );
    personalsTable_->setItem( nPos, 2, new QTableWidgetItem( locale().toString( nMax ) ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeDotations
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeDotations()
{
    dotationsTable_->setRowCount( 0 );
    catetoriesNames_.clear();
    const tools::Resolver_ABC< DotationType >& dotations = static_.objectTypes_;
    auto it = dotations.CreateIterator();
    std::set< unsigned long > inserted;
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsAmmunition() || !inserted.insert( type.GetFamily() ).second )
            continue;
        const unsigned nPos = dotationsTable_->rowCount();
        dotationsTable_->insertRow( nPos );
        QTableWidgetItem* item =  new QTableWidgetItem( type.GetCategoryDisplay().c_str() );
        item->setCheckState( Qt::Unchecked );
        dotationsTable_->setItem( nPos, 0, item );
        dotationDelegate_->AddSpinBoxOnColumn( 1 );
        dotationsTable_->setItem( nPos, 1, new QTableWidgetItem( locale().toString( 0 ) ) );
        catetoriesNames_.push_back( type.GetCategoryName().c_str() );
    }
    dotationsTable_->setMinimumHeight( std::max< int >( dotationsTable_->rowHeight( 0 ) * 4, 20 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeAmmunitions
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeAmmunitions()
{
    munitionsFamilyTable_->setRowCount( 0 );
    AddAmmunition( eAmmunitionType_Obus,          ENT_Tr::ConvertFromAmmunitionType( eAmmunitionType_Obus, ENT_Tr::eToTr ).c_str() );
    AddAmmunition( eAmmunitionType_MissileSol,    ENT_Tr::ConvertFromAmmunitionType( eAmmunitionType_MissileSol, ENT_Tr::eToTr ).c_str() );
    AddAmmunition( eAmmunitionType_MissileAir,    ENT_Tr::ConvertFromAmmunitionType( eAmmunitionType_MissileAir, ENT_Tr::eToTr ).c_str() );
    AddAmmunition( eAmmunitionType_Mitraille,     ENT_Tr::ConvertFromAmmunitionType( eAmmunitionType_Mitraille, ENT_Tr::eToTr ).c_str() );
    AddAmmunition( eAmmunitionType_ALR,           ENT_Tr::ConvertFromAmmunitionType( eAmmunitionType_ALR, ENT_Tr::eToTr ).c_str() );
    munitionsFamilyTable_->setMinimumHeight( std::max< int >( munitionsFamilyTable_->rowHeight( 0 ) * 4, 20 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::AddAmmunition
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::AddAmmunition( unsigned nPos, const QString& label )
{
    munitionsFamilyTable_->insertRow( nPos );
    QTableWidgetItem* item = new QTableWidgetItem( label );
    item->setCheckState( Qt::Unchecked );
    munitionsFamilyTable_->setItem( nPos, 0, item );
    munitionsFamilyDelegate_->AddSpinBoxOnColumn( 1 );
    munitionsFamilyTable_->setItem( nPos, 1, new QTableWidgetItem( locale().toString( 0 ) ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeStocks
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeStocks()
{
    stocks_.clear();
    stockTable_->setRowCount( 0 );
    const SupplyStates* supplies = selected_->Retrieve< SupplyStates >();
    if( supplies )
    {
        auto it = supplies->CreateIterator();
        while( it.HasMoreElements() )
        {
            const Dotation& stock = it.NextElement();
            stocks_[ stock.type_->GetName().c_str() ] = &stock;
            const float normalized = logistic_helpers::ComputeNormalizedQuantity( static_, *selected_, *stock.type_, stock.quantity_ );
            const unsigned int nPos = stockTable_->rowCount();
            stockTable_->insertRow( nPos );
            QTableWidgetItem* item = new QTableWidgetItem( stock.type_->GetName().c_str() );
            item->setCheckState( Qt::Unchecked );
            stockTable_->setItem( nPos, 0, item );
            stockTable_->setItem( nPos, 1, new QTableWidgetItem( locale().toString( stock.quantity_ ) ) );
            stockTable_->setItem( nPos, 2, new QTableWidgetItem( locale().toString( normalized, 'f', 2 ) ) );
        }
        stockDelegate_->AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
        stockDelegate_->AddDoubleSpinBoxOnColumn( 2, 0, std::numeric_limits< int >::max(), 0.5 );
        stockTable_->setMinimumHeight( std::max< int >( stockTable_->rowHeight( 0 ) * 4, 20 ) );
        stockGroupBox_->setVisible( true );
    }
    else
        stockGroupBox_->setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Show
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Show()
{
    if( !selected_ )
        return;
    hide();
    InitializeEquipments ();
    InitializePersonal   ();
    InitializeDotations  ();
    InitializeAmmunitions();
    InitializeStocks     ();
    show();
}

namespace
{
    std::string CreateName( const std::string& str, int& index )
    {
        return QString( (str + " %L1" ).c_str() ).arg( index++ ).toStdString();
    }

    void Fill( ParameterList& list, QTableWidget& table, const std::string& name, const std::string& identifier, const std::string& quantity,
               bool checkState, const std::function< unsigned int( int ) >& idFunctor )
    {
        int index = 1;
        for( int nRow = 0; nRow < table.rowCount(); ++nRow )
        {
            const auto item = table.item( nRow, 0 );
            if( !item || item->text().isEmpty() )
                // Skip empty trailing entry
                continue;

            if( !checkState || ( checkState && item->checkState() == Qt::Checked ) )
            {
                ParameterList& personalList = list.AddList( CreateName( name, index ) );
                personalList.AddIdentifier( identifier, idFunctor( nRow ) );
                personalList.AddQuantity( quantity, table.locale().toInt( table.item( nRow, 1 )->text() ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillPersonal
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillPersonal( ParameterList& list )
{
    Fill( list, *personalsTable_, "Human", "Rank", "Number", true,
          [&]( int nRow ){ return nRow; } );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillEquipments
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillEquipments( actions::parameters::ParameterList& list )
{
    if( equipmentsTable_->rowCount() > 1 )
        Fill( list, *equipmentsTable_, "Equipment", "Equipment", "Number", false,
              [&]( int nRow ){ return equipments_[ equipmentsTable_->item( nRow, 0 )->text() ]->type_.GetId(); } );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillDotations
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillDotations( actions::parameters::ParameterList& list )
{
    Fill( list, *dotationsTable_, "Dotation", "Dotation", "Number", true,
          [&]( int nRow ){ return ENT_Tr::ConvertToDotationType( catetoriesNames_[ nRow ].toStdString(), ENT_Tr::eToTr ); } );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillAmmunitions
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillAmmunitions( actions::parameters::ParameterList& list )
{
    Fill( list, *munitionsFamilyTable_, "Ammo", "Ammo", "Number", true,
          [&]( int nRow ){ return nRow; } );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillStocks
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillStocks( actions::parameters::ParameterList& list )
{
    Fill( list, *stockTable_, "Stock", "Stock", "Number", true,
          [&]( int nRow ){ return stocks_[ stockTable_->item( nRow, 0 )->text() ]->type_->GetId(); } );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Validate
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Validate()
{
    if( ! selected_ )
        return;

    accept();

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& >( static_.types_ ).Get( "partial_recovery" );
    std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );

    auto it = actionType.CreateIterator();
    parameters::ParameterList* equipments = new parameters::ParameterList( it.NextElement() );
    parameters::ParameterList* humans = new parameters::ParameterList( it.NextElement() );
    parameters::ParameterList* dotations = new parameters::ParameterList( it.NextElement() );
    parameters::ParameterList* ammo = new parameters::ParameterList( it.NextElement() );
    parameters::ParameterList* stocks = new parameters::ParameterList( it.NextElement() );

    action->AddParameter( *equipments );
    action->AddParameter( *humans );
    action->AddParameter( *dotations );
    action->AddParameter( *ammo );
    action->AddParameter( *stocks );

    FillEquipments( *equipments );
    FillPersonal( *humans );
    FillDotations( *dotations );
    FillAmmunitions( *ammo );
    FillStocks( *stocks );

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( controllers_.controller_, selected_, false ) );
    actionsModel_.Publish( *action );

    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Reject
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Reject()
{
    reject();
    selected_ = 0;
}

namespace
{
    void OnChange( QTableWidget& table, int nRow, int nCol, bool isVisible )
    {
        if( nCol != 1 || !isVisible )
            return;
        table.item( nRow, 0 )->setCheckState( Qt::Checked );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnDotationChanged
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnDotationChanged( int nRow, int nCol )
{
    OnChange( *dotationsTable_, nRow, nCol, isVisible() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnMunitionFamilyChanged
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnMunitionFamilyChanged( int nRow, int nCol )
{
    OnChange( *munitionsFamilyTable_, nRow, nCol, isVisible() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnPersonalChanged
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnPersonalChanged( int nRow, int nCol )
{
    OnChange( *personalsTable_, nRow, nCol, isVisible() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FilterEquipmentList
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
QStringList LogisticSupplyRecompletionDialog::FilterEquipmentList( int index ) const
{
    QStringList result( equipmentsList_ );
    for( int i = 0; i < equipmentsTable_->rowCount(); ++i )
    {
        if( QTableWidgetItem* item = equipmentsTable_->item( i, 0 ) )
        {
            QString val = equipmentsTable_->item( i, 0 )->text();
            if( i != index && !item->text().isEmpty() )
                result.remove( item->text() );
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnMunitionChanged
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnEquipmentChanged( int nRow, int nCol )
{
    if( nCol != 0 && nCol != 1 )
        return;

    if( nCol == 1 && ( !equipmentsTable_->item( nRow, 0 ) || ( equipmentsTable_->item( nRow, 0 ) && equipmentsTable_->item( nRow, 0 )->text().isEmpty() ) ) )
    {
        equipmentsTable_->item( nRow, 1 )->setText( "" );
        return;
    }

    int comBoIndex = equipmentsList_.indexOf( equipmentsTable_->item( nRow, 0 )->text() );
    if( comBoIndex == 0 )
    {
        // if not last row, delete empty row
        if( nRow != equipmentsTable_->rowCount() - 1 )
        {
            equipmentsTable_->removeRow( nRow );
            // select last row quantity field
            equipmentsTable_->setCurrentCell( equipmentsTable_->rowCount() - 1, 1 );
        }
    }
    else if( comBoIndex > 0 )
    {
        // if last row is set, add a new row to table
        int nMax = equipmentsMax_[ comBoIndex - 1 ];
        if( nCol == 0 )
        {
            equipmentDelegate_->AddSpinBox( nRow , nRow, 1, 1, 0, nMax );
            equipmentsTable_->setItem( nRow, 1, new QTableWidgetItem( locale().toString( 0 ) ) );
        }
        if( nRow == equipmentsTable_->rowCount() - 1 )
            equipmentsTable_->insertRow( equipmentsTable_->rowCount() );
        equipmentsTable_->setItem( nRow, 2, new QTableWidgetItem( locale().toString( equipmentsMax_[ comBoIndex - 1 ] ) ) );
    }
    if( nCol == 0 )
        for( int i = 0; i < equipmentsTable_->rowCount(); ++i )
            equipmentDelegate_->AddComboBox( i, i, 0, 0, FilterEquipmentList( i ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnStockChanged
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnStockChanged( int nRow, int nCol )
{
    if( nCol == 0 || !isVisible() )
        return;
    stockTable_->blockSignals( true );
    stockTable_->item( nRow, 0 )->setCheckState( Qt::Checked );
    const Dotation& dotation = *stocks_[ stockTable_->item( nRow, 0 )->text() ];
    logistic_helpers::T_Requirements requirements;
    logistic_helpers::ComputeLogisticConsumptions( static_, *selected_, dotation.type_->GetLogisticSupplyClass(), requirements, false );
    const int consumption = requirements[ dotation.type_ ];
    if( nCol == 1 )
    {
        const double quantity = locale().toDouble( stockTable_->item( nRow, 1 )->text() );
        const double normalized = consumption > 0 ? quantity / consumption : 0;
        stockTable_->setItem( nRow, 2, new QTableWidgetItem( locale().toString( normalized, 'f', 2 ) ) );
    }
    else
    {
        const double normalized = locale().toDouble( stockTable_->item( nRow, 2 )->text() );
        const int quantity = static_cast< int >( consumption * normalized );
        stockTable_->setItem( nRow, 1, new QTableWidgetItem( locale().toString( quantity ) ) );
    }
    stockTable_->blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::NotifyContextMenu
// Created: SBO 2006-08-10
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( agent ) )
        return;
    selected_ = &agent;
    kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
    subMenu->insertItem( tr( "Partial recompletion" ), this, SLOT( Show() ) );
}
