// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"

#include "LogisticSupplyRecompletionDialog.h"
#include "moc_LogisticSupplyRecompletionDialog.cpp"

#include "App.h"
#include "ASN_Messages.h"
#include "Model.h"
#include "Agent.h"
#include "Equipments.h"
#include "Troops.h"
#include "Dotations.h"
#include "Dotation.h"
#include "DotationType.h"
#include "ObjectTypes.h"
#include "SupplyStates.h"
#include "Equipments.h"
#include "Equipment.h"
#include "EquipmentType.h"


#include <qtable.h>
#include <qspinbox.h>

class SpinTableItem : public QTableItem
{
public:
    //! @name Constructors/Destructor
    //@{
    SpinTableItem( QTable* table, int minValue, int maxValue, int step = 1 )
        : QTableItem( table, QTableItem::WhenCurrent )
        , minValue_( minValue )
        , maxValue_( maxValue )
        , step_( step )
    {}
    virtual ~SpinTableItem() {}
    //@}

    //! @name Operations
    //@{
    QWidget* createEditor() const
    {
        QSpinBox* spinBox = new QSpinBox( minValue_, maxValue_, step_, table()->viewport(), "spintableitem" );
        QObject::connect( spinBox, SIGNAL( valueChanged( int ) ), table(), SLOT( doValueChanged() ) );
    	
        if( !text().isNull() )
            spinBox->setValue( text().toInt() );
        else
            spinBox->setValue( 0 );
        return spinBox;
    }

    void setContentFromEditor( QWidget* widget )
    {
        if( widget->inherits( "QSpinBox" ) )
            setText( static_cast< QSpinBox* >( widget )->text() );
        else
            QTableItem::setContentFromEditor( widget );
    }
    //@}

private:
    //! @name Member data
    //@{
	int minValue_;
    int maxValue_;
    int step_;
    //@}
};

class ComboTableItem : public QComboTableItem
{
public:
    ComboTableItem( QTable* table, const QStringList& list )
        : QComboTableItem( table, list )
        , list_( list ) {}
    virtual ~ComboTableItem() {}

    QWidget* createEditor() const
    {
        QStringList list( list_ );
        for( int r = 0; r < table()->numRows(); ++r )
        {
            const QString text = table()->text( r, col() );
            if( r != row() && !text.isEmpty() )
                list.remove( text );
        }
        const_cast< ComboTableItem* >( this )->setStringList( list );
        return QComboTableItem::createEditor();
    }

private:
    QStringList list_;
};

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog constructor
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
LogisticSupplyRecompletionDialog::LogisticSupplyRecompletionDialog( QWidget* parent, Controllers& controllers, const Model& model )
    : QDialog              ( parent, "Recompletement" )
    , model_               ( model )
    , agent_               ( controllers )
{
    setCaption( tr( "Recompletement" ) );
    resize( 280, 430 );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 5 );
    mainLayout->setMargin( 5 );

    // Equipment and personal
    QGroupBox* equiPersoGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Equipement - Personnel" ), this );
    equiPersoGroupBox->setFlat( true );

    equipmentsTable_ = new QTable( 0, 3, equiPersoGroupBox );
    equipmentsTable_->horizontalHeader()->setLabel( 0, tr( "Equipement" ) );
    equipmentsTable_->horizontalHeader()->setLabel( 1, tr( "Nombre" ) );
    equipmentsTable_->horizontalHeader()->setLabel( 2, tr( "Max" ) );
    equipmentsTable_->setColumnWidth( 0, equipmentsTable_->columnWidth( 0 ) + 20 );
    equipmentsTable_->setColumnWidth( 1, 50 );
    equipmentsTable_->setColumnWidth( 2, 50 );
    equipmentsTable_->setColumnReadOnly( 2, true );
    equipmentsTable_->setLeftMargin( 0 );
    equipmentsTable_->setShowGrid( false );

    personalsTable_ = new QTable( 0, 4, equiPersoGroupBox );
    personalsTable_->horizontalHeader()->setLabel( 0, "" );
    personalsTable_->horizontalHeader()->setLabel( 1, tr( "Personnel" ) );
    personalsTable_->horizontalHeader()->setLabel( 2, tr( "Quantit�" ) );
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
    QGroupBox* dotationsGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Dotations" ), this );
    dotationsGroupBox->setFlat( true );

    dotationsTable_ = new QTable( 0, 3, dotationsGroupBox );
    dotationsTable_->horizontalHeader()->setLabel( 0, "" );
    dotationsTable_->horizontalHeader()->setLabel( 1, tr( "Dotation" ) );
    dotationsTable_->horizontalHeader()->setLabel( 2, tr( "Quantit�" ) );
    dotationsTable_->setColumnWidth( 0, 20 );
    dotationsTable_->setColumnWidth( 2, 60 );
    dotationsTable_->setColumnReadOnly( 1, true );
    dotationsTable_->setLeftMargin( 0 );
    dotationsTable_->setShowGrid( false );

    // Munitions Families
    munitionsFamilyTable_ = new QTable( 0, 3, dotationsGroupBox );
    munitionsFamilyTable_->horizontalHeader()->setLabel( 0, "" );
    munitionsFamilyTable_->horizontalHeader()->setLabel( 1, tr( "Munition" ) );
    munitionsFamilyTable_->horizontalHeader()->setLabel( 2, tr( "Quantit�" ) );
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
    stockTable_->horizontalHeader()->setLabel( 1, tr( "Dotation" ) );
    stockTable_->horizontalHeader()->setLabel( 2, tr( "Quantit�" ) );
    stockTable_->setColumnWidth( 0, 20 );
    stockTable_->setColumnWidth( 2, 60 );
    stockTable_->setColumnReadOnly( 1, true );
    stockTable_->setLeftMargin( 0 );
    stockTable_->setShowGrid( false );

    mainLayout->addWidget( stockGroupBox );

    // ok / cancel butons
    QHBoxLayout* buttonLayout = new QHBoxLayout( mainLayout );
    QPushButton* okButton     = new QPushButton( tr("OK")    , this );
    QPushButton* cancelButton = new QPushButton( tr("Annuler"), this );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeEquipments
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeEquipments( const Agent& agent )
{
    equipmentsList_.clear();
    equipmentsList_.append( "" );
    equipmentsMax_.clear();
    const Equipments& equipments = agent.Get< Equipments >();
    Iterator< const Equipment& > it = equipments.CreateIterator();
    QStringList equipmentList;
    while( it.HasMoreElements() )
    {
        const Equipment& equipment = it.NextElement();
        equipmentsList_.append( equipment.GetName().c_str() );
        equipments_[ equipment.GetName().c_str() ] = &equipment;
        equipmentsMax_.push_back( equipment.Total() );
    }
    equipmentsTable_->setNumRows( 0 );
    equipmentsTable_->insertRows( 0, 1 );
    equipmentsTable_->setItem( 0, 0, new ComboTableItem( equipmentsTable_, equipmentsList_ ) );
    equipmentsTable_->setText( 0, 1, "0" );
    equipmentsTable_->setMinimumHeight( equipmentsTable_->rowHeight( 0 ) * 4 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializePersonal
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializePersonal( const Agent& agent )
{
    const Troops& troops = agent.Get< Troops >();
    personalsTable_->setNumRows( 0 );
    AddPersonal( 0, tr( "officier" ), troops.humans_[ eTroopHealthStateTotal ].officers_ );
    AddPersonal( 1, tr( "sous-officier" ), troops.humans_[ eTroopHealthStateTotal ].subOfficers_ );
    AddPersonal( 2, tr( "mdr" ), troops.humans_[ eTroopHealthStateTotal ].troopers_ );
    personalsTable_->setMinimumHeight( personalsTable_->rowHeight( 0 ) * 5 );
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
    personalsTable_->setItem( nPos, 2, new SpinTableItem( personalsTable_, 0, nMax, 1 ) );
    personalsTable_->setText( nPos, 3, QString::number( nMax ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeDotations
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeDotations( const Agent& /*agent*/ )
{
    dotationsTable_->setNumRows( 0 );
    const Resolver< DotationType >& dotations = model_.objectTypes_;
    Iterator< const DotationType& > it = dotations.CreateIterator();
    std::set< unsigned long > inserted;
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsAmmunition() || !inserted.insert( type.GetFamily() ).second )
            continue;
        const unsigned nPos = dotationsTable_->numRows();
        dotationsTable_->insertRows( nPos, 1 );
        dotationsTable_->setItem( nPos, 0, new QCheckTableItem( dotationsTable_, 0 ) );
        dotationsTable_->setText( nPos, 1, type.GetName().c_str() );
        dotationsTable_->setItem( nPos, 2, new SpinTableItem( dotationsTable_, 0, 100, 1 ) );
    }
    dotationsTable_->setMinimumHeight( dotationsTable_->rowHeight( 0 ) * 5 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeAmmunitions
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeAmmunitions( const Agent& /*agent*/ )
{
    munitionsFamilyTable_->setNumRows( 0 );
    AddAmmunition( 0, tr( "Obus" ) );
    AddAmmunition( 1, tr( "Missile Air" ) );
    AddAmmunition( 2, tr( "Missile Sol" ) );
    AddAmmunition( 3, tr( "Mitraille" ) );
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
    munitionsFamilyTable_->setItem( nPos, 2, new SpinTableItem( munitionsFamilyTable_, 0, 100, 1 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeSupplies
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeSupplies( const Agent& agent )
{
    stocks_.clear();
    stockTable_->setNumRows( 0 );
    const SupplyStates* supplies = agent.Retrieve< SupplyStates >();
    bool show = false;
    if( supplies )
    {
        stockTable_->setNumRows( 0 );
        stockTable_->setMinimumHeight( stockTable_->rowHeight( 0 ) * 5 );
        
        Iterator< const Dotation& > it = supplies->CreateIterator();
        while( it.HasMoreElements() )
        {
            const Dotation& stock = it.NextElement();
            const unsigned nPos = stockTable_->numRows();
            stockTable_->insertRows( nPos, 1 );
            stockTable_->setItem( nPos, 0, new QCheckTableItem( stockTable_, 0 ) );
            stockTable_->setText( nPos, 1, stock.type_->GetCategory().c_str() );
            stockTable_->setText( nPos, 2, QString::number( stock.quantity_ ) );
            stocks_[ stock.type_->GetCategory().c_str() ] = &stock;
            show = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Show
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Show( const Agent& agent )
{
    agent_ = &agent;
    InitializeEquipments ( agent );
    InitializePersonal   ( agent );
    InitializeDotations  ( agent );
    InitializeAmmunitions( agent );
    InitializeSupplies   ( agent );
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillPersonal
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillPersonal( ASN1T_MagicActionRecompletementPartiel& action )
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
        action.personnels.n        = nNbrPersonals;
        action.m.personnelsPresent  = 1;

        ASN1T_RecompletementPersonnel* pAsnPersonnel = new ASN1T_RecompletementPersonnel[ nNbrPersonals ];
        action.personnels.elem = pAsnPersonnel;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < personalsTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pPersonnelItemCheckBox = static_cast< QCheckTableItem* >( personalsTable_->item( nRow, 0 ) );
            QTableItem*      pNbrItem               = personalsTable_->item( nRow, 2 );
            if( !pPersonnelItemCheckBox->isChecked() )
                continue;
            ASN1T_RecompletementPersonnel& asnPersonnel = pAsnPersonnel[ nAsnIdx ++ ];
            asnPersonnel.rang                           = (ASN1T_EnumHumanRank)nRow;
            asnPersonnel.nombre_disponible              = pNbrItem->text().toUInt();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillEquipments
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillEquipments( ASN1T_MagicActionRecompletementPartiel& action )
{
    if( equipmentsTable_->numRows() > 1 )
    {
        action.m.equipementsPresent = 1;
        uint nAsnIdx = 0;
        ASN1T_RecompletementEquipement* pAsnEquipement = new ASN1T_RecompletementEquipement[ equipmentsTable_->numRows() - 1 ];
        for( int nRow = 0; nRow < equipmentsTable_->numRows() - 1; ++nRow )
        {
            QComboTableItem* pEquipementItem  = static_cast< QComboTableItem* >( equipmentsTable_->item( nRow, 0 ) );
            QTableItem*      pNbrItem         = equipmentsTable_->item( nRow, 1 );

            ASN1T_RecompletementEquipement& asnEquipement = pAsnEquipement[ nAsnIdx ++ ];
            asnEquipement.type_equipement   = equipments_[ pEquipementItem->currentText() ]->type_.GetId();
            asnEquipement.nombre_disponible = pNbrItem->text().toUInt();
        }

        action.equipements.n    = equipmentsTable_->numRows() - 1;
        action.equipements.elem = pAsnEquipement;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillDotations
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillDotations(  ASN1T_MagicActionRecompletementPartiel& action )
{
    uint nNbrDotations = 0;
    for( int nRow = 0; nRow < dotationsTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( dotationsTable_->item( nRow, 0 ) );
        if( pCheckTableItem->isChecked() )
            ++ nNbrDotations;
    }
    if( nNbrDotations > 0 )
    {   
        action.dotations.n        = nNbrDotations;
        action.m.dotationsPresent = 1;

        ASN1T_RecompletementDotation* pAsnDotations = new ASN1T_RecompletementDotation[ nNbrDotations ];
        action.dotations.elem = pAsnDotations;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < dotationsTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pDotationItemCheckBox = static_cast< QCheckTableItem* >( dotationsTable_->item( nRow, 0 ) );
            QTableItem*      pDotationItem         = dotationsTable_->item( nRow, 1 );
            QTableItem*      pPercentageItem       = dotationsTable_->item( nRow, 2 );

            assert( pDotationItemCheckBox );
            if( !pDotationItemCheckBox->isChecked() )
                continue;

            ASN1T_RecompletementDotation& asnDotation = pAsnDotations[ nAsnIdx ++ ];
            asnDotation.famille_dotation = (ASN1T_EnumFamilleDotation)ENT_Tr::ConvertToFamilleDotation( pDotationItem->text().ascii() );
            asnDotation.pourcentage      = pPercentageItem->text().toUInt();
        }
    } 
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillAmmunitions
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillAmmunitions( ASN1T_MagicActionRecompletementPartiel& action )
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
        action.munitions.n        = nNbrMunitions;
        action.m.munitionsPresent = 1;

        ASN1T_RecompletementDotationMunition* pAsnMunitions = new ASN1T_RecompletementDotationMunition[ nNbrMunitions ];
        action.munitions.elem = pAsnMunitions;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < munitionsFamilyTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pMunitionItemCheckBox = static_cast< QCheckTableItem* >( munitionsFamilyTable_->item( nRow, 0 ) );
            QTableItem*      pMunitionItem         = munitionsFamilyTable_->item( nRow, 1 );
            QTableItem*      pPercentageItem       = munitionsFamilyTable_->item( nRow, 2 );

            assert( pMunitionItemCheckBox );
            if( !pMunitionItemCheckBox->isChecked() )
                continue;

            assert( pMunitionItem );            
            assert( pPercentageItem );

            ASN1T_RecompletementDotationMunition& asnMunition = pAsnMunitions[ nAsnIdx ++ ];
            asnMunition.famille_munition = (ASN1T_EnumFamilleMunition)ENT_Tr::ConvertToFamilleMunition( pMunitionItem->text().ascii() );
            asnMunition.pourcentage      = pPercentageItem->text().toUInt();
        }
    }   
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::FillSupplies
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::FillSupplies( ASN1T_MagicActionRecompletementPartiel& action )
{
    uint nNbrResources = 0;
    for( int nRow = 0; nRow < stockTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( stockTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrResources;
    }
    if( nNbrResources > 0 )
    {   
        action.stocks.n        = nNbrResources;
        action.m.stocksPresent = 1;

        ASN1T_RecompletementStock* pAsnStocks = new ASN1T_RecompletementStock[ nNbrResources ];
        action.stocks.elem = pAsnStocks;
        uint nAsnIdx = 0;
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

            ASN1T_RecompletementStock& asnStock = pAsnStocks[ nAsnIdx ++ ];
            asnStock.ressource_id        = stocks_[ pItem->text() ]->type_->GetId();
            asnStock.quantite_disponible = pQttyItem->text().toUInt();
        }
    } 
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Validate
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Validate()
{
    if( ! agent_ )
        return;

    ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid = agent_->GetId();

    ASN1T_MagicActionRecompletementPartiel asnMagicAction;
    asnMsg.GetAsnMsg().action.t                        = T_MsgUnitMagicAction_action_recompletement_partiel;
    asnMsg.GetAsnMsg().action.u.recompletement_partiel = &asnMagicAction;

    FillPersonal( asnMagicAction );
    FillEquipments( asnMagicAction );
    FillDotations( asnMagicAction );
    FillAmmunitions( asnMagicAction );
    FillSupplies( asnMagicAction );

    asnMsg.Send();
    if( asnMagicAction.m.dotationsPresent && asnMagicAction.dotations.n > 0 )
        delete [] asnMagicAction.dotations.elem;

    if( asnMagicAction.m.munitionsPresent && asnMagicAction.munitions.n > 0 )
        delete [] asnMagicAction.munitions.elem;

    if( asnMagicAction.m.equipementsPresent && asnMagicAction.equipements.n > 0 )
        delete [] asnMagicAction.equipements.elem;

    if( asnMagicAction.m.personnelsPresent && asnMagicAction.personnels.n > 0 )
        delete [] asnMagicAction.personnels.elem;

    if( asnMagicAction.m.stocksPresent && asnMagicAction.stocks.n > 0 )
        delete [] asnMagicAction.stocks.elem;
    agent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Reject
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Reject()
{
    agent_ = 0;
    hide();
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
            equipmentsTable_->setItem( nPos, 0, new ComboTableItem( equipmentsTable_, equipmentsList_ ) );
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
