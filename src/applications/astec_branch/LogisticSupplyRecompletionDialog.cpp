//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogisticSupplyRecompletionDialog.cpp $
// $Author: Nld $
// $Modtime: 29/07/05 11:42 $
// $Revision: 4 $
// $Workfile: LogisticSupplyRecompletionDialog.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "LogisticSupplyRecompletionDialog.h"
#include "moc_LogisticSupplyRecompletionDialog.cpp"

#include "App.h"
#include "ASN_Messages.h"
#include "Attr_Def.h"
#include "Agent.h"
#include "MT_SpinTableItem.h" // $$$$ AGE 2006-04-28: 
#include "Equipments.h"
#include "Troops.h"
#include "Dotations.h"
#include "Model.h"
#include "ObjectTypes.h"

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog constructor
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
LogisticSupplyRecompletionDialog::LogisticSupplyRecompletionDialog( QWidget* pParent, Controllers& controllers, const Model& model )
    : QDialog               ( pParent, "Recompletement" )
    , model_                ( model )
    , agent_                ( controllers )
    , pEquipmentsTable_     ( 0 )
    , pEquipmentsStringList_( 0 )
    , pDotationsTable_      ( 0 )
    , pPersonalsTable_      ( 0 )
    , pStockTable_          ( 0 )
{
    resize( 280, 430 );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setSpacing( 5 );
    pMainLayout->setMargin( 5 );

    // Equipment and personal
    pEquiPersoGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Equipement - Personnel" ), this );
    pEquiPersoGroupBox_->setFlat( true );

    pEquipmentsTable_ = new QTable( 0, 3, pEquiPersoGroupBox_ );
    pEquipmentsTable_->horizontalHeader()->setLabel( 0, tr( "Equipement" ) );
    pEquipmentsTable_->horizontalHeader()->setLabel( 1, tr( "Nombre" ) );
    pEquipmentsTable_->horizontalHeader()->setLabel( 2, tr( "Max" ) );
    pEquipmentsTable_->setColumnWidth( 0, pEquipmentsTable_->columnWidth( 0 ) + 20 );
    pEquipmentsTable_->setColumnWidth( 1, 50 );
    pEquipmentsTable_->setColumnWidth( 2, 50 );
    pEquipmentsTable_->setColumnReadOnly( 2, true );
    pEquipmentsTable_->setLeftMargin( 0 );
    pEquipmentsTable_->setShowGrid( false );

    pPersonalsTable_ = new QTable( 0, 3, pEquiPersoGroupBox_ );
    pPersonalsTable_->horizontalHeader()->setLabel( 0, "" );
    pPersonalsTable_->horizontalHeader()->setLabel( 1, tr( "Personnel" ) );
    pPersonalsTable_->horizontalHeader()->setLabel( 2, tr( "Quantité" ) );
    pPersonalsTable_->setColumnWidth( 0, 20 );
    pPersonalsTable_->setColumnWidth( 2, 60 );
    pPersonalsTable_->setColumnReadOnly( 1, true );
    pPersonalsTable_->setLeftMargin( 0 );
    pPersonalsTable_->setShowGrid( false );

    pMainLayout->addWidget( pEquiPersoGroupBox_ );

    // Dotations && !munition
    pDotationsGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Dotations" ), this );
    pDotationsGroupBox_->setFlat( true );

    pDotationsTable_ = new QTable( 0, 3, pDotationsGroupBox_ );
    pDotationsTable_->horizontalHeader()->setLabel( 0, "" );
    pDotationsTable_->horizontalHeader()->setLabel( 1, tr( "Dotation" ) );
    pDotationsTable_->horizontalHeader()->setLabel( 2, tr( "Quantité" ) );
    pDotationsTable_->setColumnWidth( 0, 20 );
    pDotationsTable_->setColumnWidth( 2, 60 );
    pDotationsTable_->setColumnReadOnly( 1, true );
    pDotationsTable_->setLeftMargin( 0 );
    pDotationsTable_->setShowGrid( false );

    // Munitions Families
    pMunitionsFamilyTable_ = new QTable( 0, 3, pDotationsGroupBox_ );
    pMunitionsFamilyTable_->horizontalHeader()->setLabel( 0, "" );
    pMunitionsFamilyTable_->horizontalHeader()->setLabel( 1, tr( "Munition" ) );
    pMunitionsFamilyTable_->horizontalHeader()->setLabel( 2, tr( "Quantité" ) );
    pMunitionsFamilyTable_->setColumnWidth( 0, 20 );
    pMunitionsFamilyTable_->setColumnWidth( 2, 60 );
    pMunitionsFamilyTable_->setColumnReadOnly( 1, true );
    pMunitionsFamilyTable_->setLeftMargin( 0 );
    pMunitionsFamilyTable_->setShowGrid( false );

    pMainLayout->addWidget( pDotationsGroupBox_ );

    // Stocks
    pStockGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Stocks" ), this );
    pStockGroupBox_->setFlat( true );

    pStockTable_ = new QTable( 0, 3, pStockGroupBox_ );
    pStockTable_->horizontalHeader()->setLabel( 0, "" );
    pStockTable_->horizontalHeader()->setLabel( 1, tr( "Dotation" ) );
    pStockTable_->horizontalHeader()->setLabel( 2, tr( "Quantité" ) );
    pStockTable_->setColumnWidth( 0, 20 );
    pStockTable_->setColumnWidth( 2, 60 );
    pStockTable_->setColumnReadOnly( 1, true );
    pStockTable_->setLeftMargin( 0 );
    pStockTable_->setShowGrid( false );

    pMainLayout->addWidget( pStockGroupBox_ );

    // ok / cancel butons
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    pButtonLayout->addWidget( pOKButton     );
    pButtonLayout->addWidget( pCancelButton );
    pOKButton->setDefault( TRUE );

    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    connect( pDotationsTable_      , SIGNAL( valueChanged( int, int ) ), SLOT( OnDotationChanged      ( int, int ) ) );
    connect( pMunitionsFamilyTable_, SIGNAL( valueChanged( int, int ) ), SLOT( OnMunitionFamilyChanged( int, int ) ) );
    connect( pEquipmentsTable_     , SIGNAL( valueChanged( int, int ) ), SLOT( OnEquipmentChanged     ( int, int ) ) );
    connect( pPersonalsTable_      , SIGNAL( valueChanged( int, int ) ), SLOT( OnPersonalChanged      ( int, int ) ) );
    connect( pStockTable_          , SIGNAL( valueChanged( int, int ) ), SLOT( OnStockChanged         ( int, int ) ) );
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
    equipmentsList_->append( "" );
    equipmentsMax_.clear();
    const Equipments& equipments = agent.Get< Equipments >();
    Iterator< Equipment > it = equipments.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Equipment& equipment = it.NextElement();
        equipmentsList_->append( equipment.GetName().c_str() );
        equipmentsMax_.push_back( equipment.Total() );
    }
    pEquipmentsTable_->setNumRows( 0 );
    pEquipmentsTable_->insertRows( 0, 1 );
    pEquipmentsTable_->setItem( 0, 0, new QComboTableItem( pEquipmentsTable_, equipmentsList_ ) );
    pEquipmentsTable_->setText( 0, 1, "0" );
    pEquipmentsTable_->setMinimumHeight( pEquipmentsTable_->rowHeight( 0 ) * 4 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializePersonal
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializePersonal( const Agent& agent )
{
    const Troops& troops = agent.Get< Troops >();
    pPersonalsTable_->setNumRows( 0 );
    {
        const nPos = 0;
        pPersonalsTable_->insertRows( nPos, 1 );
        pPersonalsTable_->setItem( nPos, 0, new QCheckTableItem( pPersonalsTable_, 0 ) );
        pPersonalsTable_->setText( nPos, 1, tr( "officier" ) );
        const unsigned nMax = troops.humans_[ eTroopHealthStateTotal ].officers_;
        pPersonalsTable_->setItem( nPos, 2, new MT_SpinTableItem( pPersonalsTable_, 0, nMax, 1 ) );
    }
    {
        const nPos = 1;
        pPersonalsTable_->insertRows( nPos, 1 );
        pPersonalsTable_->setItem( nPos, 0, new QCheckTableItem( pPersonalsTable_, 0 ) );
        pPersonalsTable_->setText( nPos, 1, tr( "sous-officier" ) );
        const unsigned nMax = troops.humans_[ eTroopHealthStateTotal ].subOfficers_;
        pPersonalsTable_->setItem( nPos, 2, new MT_SpinTableItem( pPersonalsTable_, 0, nMax, 1 ) );
    }
    {
        const nPos = 2;
        pPersonalsTable_->insertRows( nPos, 1 );
        pPersonalsTable_->setItem( nPos, 0, new QCheckTableItem( pPersonalsTable_, 0 ) );
        pPersonalsTable_->setText( nPos, 1, tr( "mdr" ) );
        const unsigned nMax = troops.humans_[ eTroopHealthStateTotal ].troopers_;
        pPersonalsTable_->setItem( nPos, 2, new MT_SpinTableItem( pPersonalsTable_, 0, nMax, 1 ) );
    }
    pPersonalsTable_->setMinimumHeight( pPersonalsTable_->rowHeight( 0 ) * 5 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeDotations
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeDotations( const Agent& agent )
{
    const Resolver< DotationType >& dotations = model_.objectTypes_;
    Iterator< const DotationType& > it = dotations.CreateIterator();
    std::set< unsigned long > inserted;
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsAmmunition() || !inserted.insert( type.GetFamily() ) )
            continue;
        const unsigned nPos = pDotationsTable_->numRows();
        pDotationsTable_->insertRows( nPos, 1 );
        pDotationsTable_->setItem( nPos, 0, new QCheckTableItem( pDotationsTable_, 0 ) );
        pDotationsTable_->setText( nPos, 1, type.GetName().c_str() );
        pDotationsTable_->setItem( nPos, 2, new MT::MT_SpinTableItem( pDotationsTable_, 0, 100, 1 ) );
    }
    pDotationsTable_->setMinimumHeight( pDotationsTable_->rowHeight( 0 ) * 5 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeAmmunitions
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeAmmunitions( const Agent& agent )
{
    // $$$$ AGE 2006-04-28: 
    pMunitionsFamilyTable_->setNumRows( 0 );
    for( uint eType = 0; eType < ( uint )eNbrFamilleMunition; ++eType )
    {
        uint nPos = pMunitionsFamilyTable_->numRows();
        pMunitionsFamilyTable_->insertRows( nPos, 1 );
        pMunitionsFamilyTable_->setItem( nPos, 0, new QCheckTableItem( pMunitionsFamilyTable_, 0 ) );
        pMunitionsFamilyTable_->setText( nPos, 1, ENT_Tr::ConvertFromFamilleMunition( ( E_FamilleMunition )eType ).c_str() );
        pMunitionsFamilyTable_->setItem( nPos, 2, new MT::MT_SpinTableItem( pMunitionsFamilyTable_, 0, 100, 1 ) );
    }
    pMunitionsFamilyTable_->setMinimumHeight( pMunitionsFamilyTable_->rowHeight( 0 ) * 5 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::InitializeSupplies
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::InitializeSupplies( const Agent& agent )
{
    if( agent.pSupplyData_ )
    {
        T_ResourceQty_Map& supplyMap = agent.pSupplyData_->stocks_;
        pStockTable_->setNumRows( 0 );
        pStockTable_->setMinimumHeight( pStockTable_->rowHeight( 0 ) * 5 );
        for( CIT_ResourceQty_Map it = supplyMap.begin(); it != supplyMap.end(); ++it )
        {
            uint nPos = pStockTable_->numRows();
            pStockTable_->insertRows( nPos, 1 );
            pStockTable_->setItem( nPos, 0, new QCheckTableItem( pStockTable_, 0 ) );
            pStockTable_->setText( nPos, 1, App::GetApp().GetResourceName( it->first ).c_str() );
            pStockTable_->setText( nPos, 2, QString::number( it->second ) );
        }
        if( supplyMap.size() > 0 )
            pStockGroupBox_->show();
        else
            pStockGroupBox_->hide();
    }
    else
        pStockGroupBox_->hide();
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
// Name: LogisticSupplyRecompletionDialog::Validate
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Validate()
{
    assert( pAgent_ );

    ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid = pAgent_->GetID();

    ASN1T_MagicActionRecompletementPartiel asnMagicAction;

    asnMsg.GetAsnMsg().action.t                        = T_MsgUnitMagicAction_action_recompletement_partiel;
    asnMsg.GetAsnMsg().action.u.recompletement_partiel = &asnMagicAction;

    // Personnels
    {
    uint nNbrPersonals = 0;
    for( int nRow = 0; nRow < pPersonalsTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pPersonalsTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrPersonals;
    }
    if( nNbrPersonals > 0 )
    {   
        asnMagicAction.personnels .n        = nNbrPersonals;
        asnMagicAction.m.personnelsPresent  = 1;

        ASN1T_RecompletementPersonnel* pAsnPersonnel = new ASN1T_RecompletementPersonnel[ nNbrPersonals ];
        asnMagicAction.personnels.elem = pAsnPersonnel;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < pPersonalsTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pPersonnelItemCheckBox = static_cast< QCheckTableItem* >( pPersonalsTable_->item( nRow, 0 ) );
            QTableItem*      pRangItem              = pPersonalsTable_->item( nRow, 1 );
            QTableItem*      pNbrItem               = pPersonalsTable_->item( nRow, 2 );

            assert( pPersonnelItemCheckBox );
            if( !pPersonnelItemCheckBox->isChecked() )
                continue;

            assert( pRangItem );            
            assert( pNbrItem );

            ASN1T_RecompletementPersonnel& asnPersonnel = pAsnPersonnel[ nAsnIdx ++ ];
            asnPersonnel.rang                           = (ASN1T_EnumHumanRank)ENT_Tr::ConvertToHumanRank( pRangItem->text().ascii() );
            asnPersonnel.nombre_disponible              = pNbrItem->text().toUInt();
        }
    }   
    }
    
    // Equipements
    if( pEquipmentsTable_->numRows() > 1 )
    {
        asnMagicAction.m.equipementsPresent = 1;
        uint nAsnIdx = 0;
        ASN1T_RecompletementEquipement* pAsnEquipement = new ASN1T_RecompletementEquipement[ pEquipmentsTable_->numRows() - 1 ];
        for( int nRow = 0; nRow < pEquipmentsTable_->numRows() - 1; ++nRow )
        {
            QComboTableItem* pEquipementItem  = static_cast< QComboTableItem* >( pEquipmentsTable_->item( nRow, 0 ) );
            QTableItem*      pNbrItem         = pEquipmentsTable_->item( nRow, 1 );

            assert( pEquipementItem );
            assert( pNbrItem );

            ASN1T_RecompletementEquipement& asnEquipement = pAsnEquipement[ nAsnIdx ++ ];
            asnEquipement.type_equipement   = App::GetApp().GetEquipementID( pEquipementItem->currentText().ascii() );
            asnEquipement.nombre_disponible = pNbrItem->text().toUInt();
        }

        asnMagicAction.equipements.n    = pEquipmentsTable_->numRows() - 1;
        asnMagicAction.equipements.elem = pAsnEquipement;
    }

    // Dotations
    {
    uint nNbrDotations = 0;
    for( int nRow = 0; nRow < pDotationsTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pDotationsTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrDotations;
    }
    if( nNbrDotations > 0 )
    {   
        asnMagicAction.dotations.n        = nNbrDotations;
        asnMagicAction.m.dotationsPresent = 1;

        ASN1T_RecompletementDotation* pAsnDotations = new ASN1T_RecompletementDotation[ nNbrDotations ];
        asnMagicAction.dotations.elem = pAsnDotations;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < pDotationsTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pDotationItemCheckBox = static_cast< QCheckTableItem* >( pDotationsTable_->item( nRow, 0 ) );
            QTableItem*      pDotationItem         = pDotationsTable_->item( nRow, 1 );
            QTableItem*      pPercentageItem       = pDotationsTable_->item( nRow, 2 );

            assert( pDotationItemCheckBox );
            if( !pDotationItemCheckBox->isChecked() )
                continue;

            assert( pDotationItem );            
            assert( pPercentageItem );

            ASN1T_RecompletementDotation& asnDotation = pAsnDotations[ nAsnIdx ++ ];
            asnDotation.famille_dotation = (ASN1T_EnumFamilleDotation)ENT_Tr::ConvertToFamilleDotation( pDotationItem->text().ascii() );
            asnDotation.pourcentage      = pPercentageItem->text().toUInt();
        }
    }   
    }

    // Munitions
    {
    uint nNbrMunitions = 0;
    for( int nRow = 0; nRow < pMunitionsFamilyTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pMunitionsFamilyTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrMunitions;
    }
    if( nNbrMunitions > 0 )
    {   
        asnMagicAction.munitions.n        = nNbrMunitions;
        asnMagicAction.m.munitionsPresent = 1;

        ASN1T_RecompletementDotationMunition* pAsnMunitions = new ASN1T_RecompletementDotationMunition[ nNbrMunitions ];
        asnMagicAction.munitions.elem = pAsnMunitions;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < pMunitionsFamilyTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pMunitionItemCheckBox = static_cast< QCheckTableItem* >( pMunitionsFamilyTable_->item( nRow, 0 ) );
            QTableItem*      pMunitionItem         = pMunitionsFamilyTable_->item( nRow, 1 );
            QTableItem*      pPercentageItem       = pMunitionsFamilyTable_->item( nRow, 2 );

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

    // Stocks
    {
    uint nNbrResources = 0;
    for( int nRow = 0; nRow < pStockTable_->numRows(); ++nRow )
    {
        QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pStockTable_->item( nRow, 0 ) );
        assert( pCheckTableItem );
        if( pCheckTableItem->isChecked() )
            ++ nNbrResources;
    }
    if( nNbrResources > 0 )
    {   
        asnMagicAction.stocks.n        = nNbrResources;
        asnMagicAction.m.stocksPresent = 1;

        ASN1T_RecompletementStock* pAsnStocks = new ASN1T_RecompletementStock[ nNbrResources ];
        asnMagicAction.stocks.elem = pAsnStocks;
        uint nAsnIdx = 0;
        for( int nRow = 0; nRow < pStockTable_->numRows(); ++nRow )
        {
            QCheckTableItem* pItemCheckBox = static_cast< QCheckTableItem* >( pStockTable_->item( nRow, 0 ) );
            QTableItem*      pItem         = pStockTable_->item( nRow, 1 );
            QTableItem*      pQttyItem     = pStockTable_->item( nRow, 2 );

            assert( pItemCheckBox );
            if( !pItemCheckBox->isChecked() )
                continue;

            assert( pItem );            
            assert( pQttyItem );

            ASN1T_RecompletementStock& asnStock = pAsnStocks[ nAsnIdx ++ ];
            asnStock.ressource_id        = (ASN1T_TypeDotation)App::GetApp().GetRessourceID( pItem->text().ascii() );
            asnStock.quantite_disponible = pQttyItem->text().toUInt();
        }
    } 
    }

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

    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyRecompletionDialog::Reject
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void LogisticSupplyRecompletionDialog::Reject()
{
    pAgent_ = 0;
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
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pDotationsTable_->item( nRow, 0 ) );
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
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pMunitionsFamilyTable_->item( nRow, 0 ) );
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
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pPersonalsTable_->item( nRow, 0 ) );
    assert( pCheckTableItem );
    pCheckTableItem->setChecked( true );
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

    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( pEquipmentsTable_->item( nRow, 0 ) );
    assert( pComboTableItem );

    // update quantity colum to bound it to max value
    QTableItem* pTableItem = static_cast< QTableItem* >( pEquipmentsTable_->item( nRow, 1 ) );
    assert( pTableItem );
    int nMax = 0;
    if( pComboTableItem->currentItem() > 0 )
        nMax = equipmentsMax_[ pComboTableItem->currentItem() - 1 ];
    if( pTableItem->text().toInt() > nMax )
    {
        QString strMax;
        strMax.setNum( nMax );
        pEquipmentsTable_->setText( nRow, 1, strMax );
    }

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( nRow != pEquipmentsTable_->numRows() - 1 )
        {
            pEquipmentsTable_->removeRow( nRow );
            // select last row quantity field
            pEquipmentsTable_->setCurrentCell( pEquipmentsTable_->numRows() - 1, 1 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( nRow == pEquipmentsTable_->numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            int nCurrentItem = pComboTableItem->currentItem();
            uint nPos = nRow + 1;
            pEquipmentsTable_->insertRows( nPos, 1 );
            pEquipmentsTable_->setItem( nPos, 0, new QComboTableItem( pEquipmentsTable_, *pEquipmentsStringList_ ) );
            pEquipmentsTable_->setText( nPos, 1, "0" );
            // need to set again the combo box selected element
            pComboTableItem->setCurrentItem( nCurrentItem );
        }
        // select quantity field
        pEquipmentsTable_->setCurrentCell( nRow, 1 );
        QString strMax;
        strMax.setNum( equipmentsMax_[ pComboTableItem->currentItem() - 1 ] );
        pEquipmentsTable_->setText( nRow, 2, strMax );
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
    QCheckTableItem* pCheckTableItem = static_cast< QCheckTableItem* >( pStockTable_->item( nRow, 0 ) );
    assert( pCheckTableItem );
    pCheckTableItem->setChecked( true );
}
