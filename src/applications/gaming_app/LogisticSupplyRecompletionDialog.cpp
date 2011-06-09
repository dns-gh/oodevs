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
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "gaming/Dotations.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "gaming/Troops.h"
#include "gaming/tools.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "clients_gui/SpinTableItem.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"

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
    resize( 280, 430 );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 5 );
    mainLayout->setMargin( 5 );

    // Equipment and personal
    QGroupBox* equiPersoGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Equipment - Crew" ), this );
    equiPersoGroupBox->setFlat( true );

    equipmentsTable_ = new QTable( 0, 3, equiPersoGroupBox );
    equipmentsTable_->horizontalHeader()->setLabel( 0, tr( "Equipment" ) );
    equipmentsTable_->horizontalHeader()->setLabel( 1, tr( "Count" ) );
    equipmentsTable_->horizontalHeader()->setLabel( 2, tr( "Max" ) );
    equipmentsTable_->setColumnWidth( 0, equipmentsTable_->columnWidth( 0 ) + 20 );
    equipmentsTable_->setColumnWidth( 1, 50 );
    equipmentsTable_->setColumnWidth( 2, 50 );
    equipmentsTable_->setColumnReadOnly( 2, true );
    equipmentsTable_->setLeftMargin( 0 );
    equipmentsTable_->setShowGrid( false );

    personalsTable_ = new QTable( 0, 4, equiPersoGroupBox );
    personalsTable_->horizontalHeader()->setLabel( 0, "" );
    personalsTable_->horizontalHeader()->setLabel( 1, tr( "Crew" ) );
    personalsTable_->horizontalHeader()->setLabel( 2, tr( "Count" ) );
    personalsTable_->horizontalHeader()->setLabel( 3, tr( "Max" ) );
    personalsTable_->setColumnWidth( 0, 20 );
    personalsTable_->setColumnWidth( 2, 60 );
    personalsTable_->setColumnWidth( 3, 40 );
    personalsTable_->setColumnReadOnly( 1, true );
    personalsTable_->setColumnReadOnly( 3, true );
    personalsTable_->setLeftMargin( 0 );
    personalsTable_->setShowGrid( false );

    mainLayout->addWidget( equiPersoGroupBox );

    // Dotations && !munition
    QGroupBox* dotationsGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Resources" ), this );
    dotationsGroupBox->setFlat( true );

    dotationsTable_ = new QTable( 0, 3, dotationsGroupBox );
    dotationsTable_->horizontalHeader()->setLabel( 0, "" );
    dotationsTable_->horizontalHeader()->setLabel( 1, tr( "Resource" ) );
    dotationsTable_->horizontalHeader()->setLabel( 2, tr( "Quantity %" ) );
    dotationsTable_->setColumnWidth( 0, 20 );
    dotationsTable_->setColumnWidth( 2, 60 );
    dotationsTable_->setColumnReadOnly( 1, true );
    dotationsTable_->setLeftMargin( 0 );
    dotationsTable_->setShowGrid( false );

    // Munitions Families
    munitionsFamilyTable_ = new QTable( 0, 3, dotationsGroupBox );
    munitionsFamilyTable_->horizontalHeader()->setLabel( 0, "" );
    munitionsFamilyTable_->horizontalHeader()->setLabel( 1, tr( "Ammo" ) );
    munitionsFamilyTable_->horizontalHeader()->setLabel( 2, tr( "Quantity %" ) );
    munitionsFamilyTable_->setColumnWidth( 0, 20 );
    munitionsFamilyTable_->setColumnWidth( 2, 60 );
    munitionsFamilyTable_->setColumnReadOnly( 1, true );
    munitionsFamilyTable_->setLeftMargin( 0 );
    munitionsFamilyTable_->setShowGrid( false );

    mainLayout->addWidget( dotationsGroupBox );

    // Stocks
    QGroupBox* stockGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Stocks" ), this );
    stockGroupBox->setFlat( true );

    stockTable_ = new QTable( 0, 3, stockGroupBox );
    stockTable_->horizontalHeader()->setLabel( 0, "" );
    stockTable_->horizontalHeader()->setLabel( 1, tr( "Resource" ) );
    stockTable_->horizontalHeader()->setLabel( 2, tr( "Quantity %" ) );
    stockTable_->setColumnWidth( 0, 20 );
    stockTable_->setColumnWidth( 2, 60 );
    stockTable_->setColumnReadOnly( 1, true );
    stockTable_->setLeftMargin( 0 );
    stockTable_->setShowGrid( false );

    mainLayout->addWidget( stockGroupBox );

    // ok / cancel butons
    QHBoxLayout* buttonLayout = new QHBoxLayout( mainLayout );
    QPushButton* okButton     = new QPushButton( tr("Ok")    , this );
    QPushButton* cancelButton = new QPushButton( tr("Cancel"), this );
    buttonLayout->addWidget( okButton     );
    buttonLayout->addWidget( cancelButton );
    okButton->setDefault( TRUE );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    connect( dotationsTable_      , SIGNAL( valueChanged( int, int ) ), SLOT( OnDotationChanged      ( int, int ) ) );
    connect( munitionsFamilyTable_, SIGNAL( valueChanged( int, int ) ), SLOT( OnMunitionFamilyChanged( int, int ) ) );
    connect( equipmentsTable_     , SIGNAL( valueChanged( int, int ) ), SLOT( OnEquipmentChanged     ( int, int ) ) );
    connect( personalsTable_      , SIGNAL( valueChanged( int, int ) ), SLOT( OnPersonalChanged      ( int, int ) ) );
    connect( stockTable_          , SIGNAL( valueChanged( int, int ) ), SLOT( OnStockChanged         ( int, int ) ) );

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
    if( const Equipments* equipments = selected_->Retrieve< Equipments >() )
    {
        tools::Iterator< const Equipment& > it = equipments->CreateIterator();
        QStringList equipmentList;
        while( it.HasMoreElements() )
        {
            const Equipment& equipment = it.NextElement();
            equipmentsList_.append( equipment.GetName() );
            equipments_[ equipment.GetName() ] = &equipment;
            equipmentsMax_.push_back( equipment.Total() );
        }
        equipmentsTable_->setNumRows( 0 );
        equipmentsTable_->insertRows( 0, 1 );
        equipmentsTable_->setItem( 0, 0, new ExclusiveComboTableItem( equipmentsTable_, equipmentsList_ ) );
        equipmentsTable_->setText( 0, 1, "0" );
        equipmentsTable_->setMinimumHeight( equipmentsTable_->rowHeight( 0 ) * 4 );
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
        personalsTable_->setNumRows( 0 );
        AddPersonal( sword::officer, tr( "officers" ), troops->humans_[ eTroopHealthStateTotal ].officers_ );
        AddPersonal( sword::sub_officer, tr( "warrant-officers" ), troops->humans_[ eTroopHealthStateTotal ].subOfficers_ );
        AddPersonal( sword::trooper, tr( "private" ), troops->humans_[ eTroopHealthStateTotal ].troopers_ );
        personalsTable_->setMinimumHeight( personalsTable_->rowHeight( 0 ) * 5 );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::AddPersonal
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::AddPersonal( unsigned nPos, const QString& label, unsigned nMax )
{
    personalsTable_->insertRows( nPos, 1 );
    personalsTable_->setItem( nPos, 0, new QCheckTableItem( personalsTable_, 0 ) );
    personalsTable_->setText( nPos, 1, label );
    personalsTable_->setItem( nPos, 2, new SpinTableItem< int >( personalsTable_, 0, nMax, 1 ) );
    personalsTable_->setText( nPos, 3, QString::number( nMax ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeDotations
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeDotations()
{
    dotationsTable_->setNumRows( 0 );
    const tools::Resolver_ABC< DotationType >& dotations = static_.objectTypes_;
    tools::Iterator< const DotationType& > it = dotations.CreateIterator();
    std::set< unsigned long > inserted;
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsAmmunition() || !inserted.insert( type.GetFamily() ).second )
            continue;
        const unsigned nPos = dotationsTable_->numRows();
        dotationsTable_->insertRows( nPos, 1 );
        dotationsTable_->setItem( nPos, 0, new QCheckTableItem( dotationsTable_, 0 ) );
        dotationsTable_->setText( nPos, 1, type.GetCategory().c_str() );
        dotationsTable_->setItem( nPos, 2, new SpinTableItem< int >( dotationsTable_, 0, 100, 1 ) );
    }
    dotationsTable_->setMinimumHeight( dotationsTable_->rowHeight( 0 ) * 5 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeAmmunitions
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeAmmunitions()
{
    munitionsFamilyTable_->setNumRows( 0 );
    AddAmmunition( eAmmunitionFamily_Obus, tr( "Shell" ) );
    AddAmmunition( eAmmunitionFamily_MissileAir, tr( "Air missile" ) );
    AddAmmunition( eAmmunitionFamily_MissileSol, tr( "Ground missile" ) );
    AddAmmunition( eAmmunitionFamily_Mitraille, tr( "Bullet" ) );
    munitionsFamilyTable_->setMinimumHeight( munitionsFamilyTable_->rowHeight( 0 ) * 5 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::AddAmmunition
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::AddAmmunition( unsigned nPos, const QString& label )
{
    munitionsFamilyTable_->insertRows( nPos, 1 );
    munitionsFamilyTable_->setItem( nPos, 0, new QCheckTableItem( munitionsFamilyTable_, 0 ) );
    munitionsFamilyTable_->setText( nPos, 1, label );
    munitionsFamilyTable_->setItem( nPos, 2, new SpinTableItem< int >( munitionsFamilyTable_, 0, 100, 1 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeSupplies
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeSupplies()
{
    stocks_.clear();
    stockTable_->setNumRows( 0 );
    const SupplyStates* supplies = selected_->Retrieve< SupplyStates >();
    bool show = false;
    if( supplies )
    {
        stockTable_->setNumRows( 0 );
        stockTable_->setMinimumHeight( stockTable_->rowHeight( 0 ) * 5 );

        tools::Iterator< const Dotation& > it = supplies->CreateIterator();
        while( it.HasMoreElements() )
        {
            const Dotation& stock = it.NextElement();
            const unsigned nPos = stockTable_->numRows();
            stockTable_->insertRows( nPos, 1 );
            stockTable_->setItem( nPos, 0, new QCheckTableItem( stockTable_, 0 ) );
            stockTable_->setText( nPos, 1, stock.type_->GetName().c_str() );
            stockTable_->setText( nPos, 2, QString::number( stock.quantity_ ) );
            stocks_[ stock.type_->GetName().c_str() ] = &stock;
            show = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Show
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Show()
{
    if( !selected_ )
        return;
    InitializeEquipments ();
    InitializePersonal   ();
    InitializeDotations  ();
    InitializeAmmunitions();
    InitializeSupplies   ();
    show();
}

namespace
{
    std::string CreateName( const std::string& str, int& index )
    {
        return QString( (str + " %1" ).c_str() ).arg( index++ ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillPersonal
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillPersonal( ParameterList& list )
{
    uint nNbrPersonals = 0;
    for( int nRow = 0; nRow < personalsTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( personalsTable_->item( nRow, 0 ) );
        if( pCheckTableItem->isChecked() )
            ++ nNbrPersonals;
    }

    if( nNbrPersonals > 0 )
    {
        int index = 1;
        for( int nRow = 0; nRow < personalsTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pPersonnelItemCheckBox = static_cast< QCheckTableItem* >( personalsTable_->item( nRow, 0 ) );
            QTableItem*      pNbrItem               = personalsTable_->item( nRow, 2 );
            if( !pPersonnelItemCheckBox->isChecked() )
                continue;

            ParameterList& personalList = list.AddList( CreateName( "Human", index ) );
            personalList.AddIdentifier( "Rank", nRow );
            personalList.AddQuantity( "Number", pNbrItem->text().toInt() );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillEquipments
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillEquipments( actions::parameters::ParameterList& list )
{
    if( equipmentsTable_->numRows() > 1 )
    {
        int index = 1;
        for( int nRow = 0; nRow < equipmentsTable_->numRows() - 1; ++nRow )
        {
            QComboTableItem* pEquipementItem  = static_cast< QComboTableItem* >( equipmentsTable_->item( nRow, 0 ) );
            QTableItem*      pNbrItem         = equipmentsTable_->item( nRow, 1 );

            ParameterList& personalList = list.AddList( CreateName( "Equipment", index ) );
            personalList.AddIdentifier( "Equipment", equipments_[ pEquipementItem->currentText() ]->type_.GetId() );
            personalList.AddQuantity( "Number", pNbrItem->text().toInt() );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillDotations
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillDotations( actions::parameters::ParameterList& list )
{
    unsigned int nNbrDotations = 0;
    for( int nRow = 0; nRow < dotationsTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( dotationsTable_->item( nRow, 0 ) );
        if( pCheckTableItem->isChecked() )
            ++ nNbrDotations;
    }
    if( nNbrDotations > 0 )
    {
        int index = 1;
        for( int nRow = 0; nRow < dotationsTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pDotationItemCheckBox = static_cast< QCheckTableItem* >( dotationsTable_->item( nRow, 0 ) );
            QTableItem*      pDotationItem         = dotationsTable_->item( nRow, 1 );
            QTableItem*      pPercentageItem       = dotationsTable_->item( nRow, 2 );

            assert( pDotationItemCheckBox );
            if( !pDotationItemCheckBox->isChecked() )
                continue;

            ParameterList& personalList = list.AddList( CreateName( "Dotation", index ) );
            personalList.AddIdentifier( "Dotation", tools::DotationFamilyFromString( pDotationItem->text() ) );
            personalList.AddQuantity( "Number", pPercentageItem->text().toInt() );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillAmmunitions
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillAmmunitions( actions::parameters::ParameterList& list )
{
    uint nNbrMunitions = 0;
    for( int nRow = 0; nRow < munitionsFamilyTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( munitionsFamilyTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrMunitions;
    }
    if( nNbrMunitions > 0 )
    {
        int index = 1;
        for( int nRow = 0; nRow < munitionsFamilyTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pMunitionItemCheckBox = static_cast< QCheckTableItem* >( munitionsFamilyTable_->item( nRow, 0 ) );
            QTableItem*      pPercentageItem       = munitionsFamilyTable_->item( nRow, 2 );

            assert( pMunitionItemCheckBox );
            if( !pMunitionItemCheckBox->isChecked() )
                continue;

            ParameterList& ammoList = list.AddList( CreateName( "Ammo", index ) );
            ammoList.AddIdentifier( "Ammo", nRow );
            ammoList.AddQuantity( "Number", pPercentageItem->text().toInt() );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillSupplies
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillSupplies( actions::parameters::ParameterList& list )
{
    unsigned int nNbrResources = 0;
    for( int nRow = 0; nRow < stockTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( stockTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrResources;
    }
    if( nNbrResources > 0 )
    {
        int index = 1;
        for( int nRow = 0; nRow < stockTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pItemCheckBox = static_cast< QCheckTableItem* >( stockTable_->item( nRow, 0 ) );
            QTableItem*      pItem         = stockTable_->item( nRow, 1 );
            QTableItem*      pQttyItem     = stockTable_->item( nRow, 2 );

            assert( pItemCheckBox );
            if( !pItemCheckBox->isChecked() )
                continue;

            assert( pItem );
            assert( pQttyItem );

            ParameterList& stockList = list.AddList( CreateName( "Stock", index ) );
            stockList.AddIdentifier( "Stock", stocks_[ pItem->text() ]->type_->GetId() );
            stockList.AddQuantity( "Number", pQttyItem->text().toInt() );
        }
    }
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
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "partial_recovery" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Partial Recovery"), true );

    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
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
    FillSupplies( *stocks );

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );

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

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnDotationChanged
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnDotationChanged( int nRow, int nCol )
{
    // do only if "quantity" field has been changed
    if( nCol != 2 )
        return;
    // check the checkbox on the same row, first cell
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( dotationsTable_->item( nRow, 0 ) );
    assert( pCheckTableItem );
    pCheckTableItem->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnMunitionFamilyChanged
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnMunitionFamilyChanged( int nRow, int nCol )
{
    // do only if "quantity" field has been changed
    if( nCol != 2 )
        return;
    // check the checkbox on the same row, first cell
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( munitionsFamilyTable_->item( nRow, 0 ) );
    assert( pCheckTableItem );
    pCheckTableItem->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnPersonalChanged
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnPersonalChanged( int nRow, int nCol )
{
    // do only if "quantity" field has been changed
    if( nCol != 2 )
        return;
    // check the checkbox on the same row, first cell
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( personalsTable_->item( nRow, 0 ) );
    assert( pCheckTableItem );
    pCheckTableItem->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FilterEquipmentList
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
QStringList LogisticSupplyRecompletionDialog::FilterEquipmentList() const
{
    QStringList result( equipmentsList_ );
    for( int i = 0; i < equipmentsTable_->numRows(); ++i )
        result.remove( equipmentsTable_->item( i, 0 )->text() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnMunitionChanged
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnEquipmentChanged( int nRow, int nCol )
{
    // do only if "equipment" or quantity field has been changed
    if( nCol != 0 && nCol != 1 )
        return;

    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( equipmentsTable_->item( nRow, 0 ) );
    assert( pComboTableItem );

    // update quantity colum to bound it to max value
    QTableItem* pTableItem = static_cast< QTableItem* >( equipmentsTable_->item( nRow, 1 ) );
    assert( pTableItem );
    int nMax = 0;
    if( pComboTableItem->currentItem() > 0 )
        nMax = equipmentsMax_[ pComboTableItem->currentItem() - 1 ];
    if( pTableItem->text().toInt() > nMax )
    {
        QString strMax;
        strMax.setNum( nMax );
        equipmentsTable_->setText( nRow, 1, strMax );
    }

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( nRow != equipmentsTable_->numRows() - 1 )
        {
            equipmentsTable_->removeRow( nRow );
            // select last row quantity field
            equipmentsTable_->setCurrentCell( equipmentsTable_->numRows() - 1, 1 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( nRow == equipmentsTable_->numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            int nCurrentItem = pComboTableItem->currentItem();
            uint nPos = nRow + 1;
            equipmentsTable_->insertRows( nPos, 1 );
            equipmentsTable_->setItem( nPos, 0, new ExclusiveComboTableItem( equipmentsTable_, equipmentsList_ ) );
            equipmentsTable_->setText( nPos, 1, "0" );
            // need to set again the combo box selected element
            pComboTableItem->setCurrentItem( nCurrentItem );
        }
        // select quantity field
        equipmentsTable_->setCurrentCell( nRow, 1 );
        QString strMax;
        strMax.setNum( equipmentsMax_[ pComboTableItem->currentItem() - 1 ] );
        equipmentsTable_->setText( nRow, 2, strMax );
    }

}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::OnStockChanged
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::OnStockChanged( int nRow, int nCol )
{
    // do only if "quantity" field has been changed
    if( nCol != 2 )
        return;
    // check the checkbox on the same row, first cell
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( stockTable_->item( nRow, 0 ) );
    assert( pCheckTableItem );
    pCheckTableItem->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::NotifyContextMenu
// Created: SBO 2006-08-10
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanDoMagic( agent ) )
    {
        selected_ = &agent;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tr( "Partial recompletion" ), this, SLOT( Show() ) );
    }
}
