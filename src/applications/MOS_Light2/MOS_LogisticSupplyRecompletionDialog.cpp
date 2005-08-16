//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogisticSupplyRecompletionDialog.cpp $
// $Author: Nld $
// $Modtime: 29/07/05 11:42 $
// $Revision: 4 $
// $Workfile: MOS_LogisticSupplyRecompletionDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"

#include "MOS_LogisticSupplyRecompletionDialog.h"
#include "moc_MOS_LogisticSupplyRecompletionDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"
#include "MOS_AgentManager.h"
#include <qtable.h>
#include "MT_SpinTableItem.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyRecompletionDialog constructor
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
MOS_LogisticSupplyRecompletionDialog::MOS_LogisticSupplyRecompletionDialog( QWidget* pParent  )
    : QDialog               ( pParent, "Recompletement" )
    , pAgent_               ( 0 )
    , pMunitionsStringList_ ( 0 )
    , pMunitionsTable_      ( 0 )
    , pDotationsTable_      ( 0 )
{
    resize( 250, 400 );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setSpacing( 5 );
    pMainLayout->setMargin( 5 );

    // Equipment and personal
    pEquiPersoGroupBox_ = new QGroupBox( 3, Qt::Horizontal, tr( "Equipement - Personnel" ), this );
    pEquiPersoGroupBox_->setFlat( true );

    pEquipmentCheckBox_ = new QCheckBox( tr( "Equipement" ), pEquiPersoGroupBox_ );
    pEquipmentSpinBox_  = new QSpinBox( 0, 100, 1, pEquiPersoGroupBox_ );
    new QLabel( " %", pEquiPersoGroupBox_ );
    
    pPersonalCheckBox_  = new QCheckBox( tr( "Personnel" ), pEquiPersoGroupBox_ );
    pPersonalSpinBox_   = new QSpinBox( 0, 100, 1, pEquiPersoGroupBox_ );
    new QLabel( " %", pEquiPersoGroupBox_ );

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

/*
    // munitions
    pMunitionsTable_ = new QTable( 0, 2, pDotationsGroupBox_ );
    pMunitionsTable_->horizontalHeader()->setLabel( 0, tr( "Munition" ) );
    pMunitionsTable_->horizontalHeader()->setLabel( 1, tr( "Quantité" ) );
    pMunitionsTable_->setColumnWidth( 0, pMunitionsTable_->columnWidth( 0 ) + 20 );
    pMunitionsTable_->setColumnWidth( 1, 60 );
    pMunitionsTable_->setLeftMargin( 0 );
    pMunitionsTable_->setShowGrid( false );
*/
    pMainLayout->addWidget( pDotationsGroupBox_ );

    // ok / cancel butons
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    pButtonLayout->addWidget( pOKButton     );
    pButtonLayout->addWidget( pCancelButton );
    pOKButton->setDefault( TRUE );

    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    connect( pDotationsTable_      , SIGNAL( valueChanged( int, int ) ), SLOT( OnDotationChanged( int, int ) ) );
    connect( pMunitionsFamilyTable_, SIGNAL( valueChanged( int, int ) ), SLOT( OnMunitionFamilyChanged( int, int ) ) );
    //connect( pMunitionsTable_      , SIGNAL( valueChanged( int, int ) ), SLOT( OnMunitionChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyRecompletionDialog destructor
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
MOS_LogisticSupplyRecompletionDialog::~MOS_LogisticSupplyRecompletionDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyRecompletionDialog::SetAgent
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyRecompletionDialog::SetAgent( const MOS_Agent& agent )
{
    assert( !pAgent_ );
    pAgent_ = &agent;

    // initialize dotation table
    pDotationsTable_->setNumRows( 0 );
    for( uint eType = 0; eType < ( uint )eNbrFamilleDotation; ++eType )
    {
        if( ( E_FamilleDotation )eType == eFamilleDotation_Munition )
            continue;
        uint nPos = pDotationsTable_->numRows();
        pDotationsTable_->insertRows( nPos, 1 );
        pDotationsTable_->setItem( nPos, 0, new QCheckTableItem( pDotationsTable_, 0 ) );
        pDotationsTable_->setText( nPos, 1, ENT_Tr::ConvertFromFamilleDotation( ( E_FamilleDotation )eType ).c_str() );
        pDotationsTable_->setItem( nPos, 2, new MT::MT_SpinTableItem( pDotationsTable_, 0, 100, 1 ) );
    }

    // initialize munitions family table
    pMunitionsFamilyTable_->setNumRows( 0 );
    for( uint eType = 0; eType < ( uint )eNbrFamilleMunition; ++eType )
    {
        uint nPos = pMunitionsFamilyTable_->numRows();
        pMunitionsFamilyTable_->insertRows( nPos, 1 );
        pMunitionsFamilyTable_->setItem( nPos, 0, new QCheckTableItem( pMunitionsFamilyTable_, 0 ) );
        pMunitionsFamilyTable_->setText( nPos, 1, ENT_Tr::ConvertFromFamilleMunition( ( E_FamilleMunition )eType ).c_str() );
        pMunitionsFamilyTable_->setItem( nPos, 2, new MT::MT_SpinTableItem( pMunitionsFamilyTable_, 0, 100, 1 ) );
    }

    /*
    // intialize munition table
    // use empty element to determine if a munition type is specified
    pMunitionsStringList_ = new QStringList();
    pMunitionsStringList_->append( "" );

    const T_ResourceQty_Map& ressources = agent.GetComposition().resources_;
    for( CIT_ResourceQty_Map it = ressources.begin(); it != ressources.end(); ++it )
    {
        const std::string strRessourceName = MOS_App::GetApp().GetResourceName( it->first );
        pMunitionsStringList_->append( strRessourceName.c_str() );
    }
    pMunitionsTable_->setNumRows( 0 );
    pMunitionsTable_->insertRows( 0, 1 );
    pMunitionsTable_->setItem( 0, 0, new QComboTableItem( pMunitionsTable_, *pMunitionsStringList_ ) );
    pMunitionsTable_->setText( 0, 1, "0" );
    */
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyRecompletionDialog::Validate
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyRecompletionDialog::Validate()
{
    assert( pAgent_ );

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid = pAgent_->GetAgentID();

    ASN1T_MagicActionRecompletementPartiel asnMagicAction;

    asnMsg.GetAsnMsg().action.t                        = T_MsgUnitMagicAction_action_recompletement_partiel;
    asnMsg.GetAsnMsg().action.u.recompletement_partiel = &asnMagicAction;

    if( pPersonalCheckBox_->isChecked() )
    {
        asnMagicAction.m.personnelsPresent = 1;
        asnMagicAction.personnels = pPersonalSpinBox_->value();
    }

    if( pEquipmentCheckBox_->isChecked() )
    {
        asnMagicAction.m.equipementsPresent = 1;
        asnMagicAction.equipements = pEquipmentSpinBox_->value();
    }

    // Dotations
    {
    uint nNbrDotations = 0;
    uint nRow;
    for( nRow = 0; nRow < pDotationsTable_->numRows(); ++nRow )
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
        for( uint nRow = 0; nRow < pDotationsTable_->numRows(); ++nRow )
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
    uint nRow;
    for( nRow = 0; nRow < pMunitionsFamilyTable_->numRows(); ++nRow )
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
        for( uint nRow = 0; nRow < pMunitionsFamilyTable_->numRows(); ++nRow )
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
/*
    // Munitions
    if( pMunitionsTable_->numRows() > 1 )
    {
        asnMagicAction.m.munitionsPresent = 1;
        uint nAsnIdx = 0;
        ASN1T_RecompletementDotationMunition* pAsnMunitions = new ASN1T_RecompletementDotationMunition[ pMunitionsTable_->numRows() - 1 ];
        for( uint nRow = 0; nRow < pMunitionsTable_->numRows() - 1; ++nRow )
        {
            QComboTableItem* pMunitionItem   = static_cast< QComboTableItem* >( pMunitionsTable_->item( nRow, 0 ) );
            QTableItem*      pPercentageItem = pDotationsTable_->item( nRow, 1 );

            assert( pMunitionItem );
            assert( pPercentageItem );

            ASN1T_RecompletementDotationMunition& asnDotation = pAsnMunitions[ nAsnIdx ++ ];
            asnDotation.munition_type = (ASN1T_EnumMunitionType)ENT_Tr::ConvertToMunitionType( pMunitionItem->currentText().ascii() );                
            asnDotation.pourcentage   = pPercentageItem->text().toUInt();
        }

        asnMagicAction.munitions.n    = pMunitionsTable_->numRows() - 1;
        asnMagicAction.munitions.elem = pAsnMunitions;
    }*/

    asnMsg.Send( 687 );

    if( asnMagicAction.m.dotationsPresent && asnMagicAction.dotations.n > 0 )
        delete [] asnMagicAction.dotations.elem;

    if( asnMagicAction.m.munitionsPresent && asnMagicAction.munitions.n > 0 )
        delete [] asnMagicAction.munitions.elem;
    
    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyRecompletionDialog::Reject
// Created: SBO 2005-07-27
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyRecompletionDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyRecompletionDialog::OnDotationChanged
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyRecompletionDialog::OnDotationChanged( int nRow, int nCol )
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
// Name: MOS_LogisticSupplyRecompletionDialog::OnMunitionFamilyChanged
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyRecompletionDialog::OnMunitionFamilyChanged( int nRow, int nCol )
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
// Name: MOS_LogisticSupplyRecompletionDialog::OnMunitionChanged
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyRecompletionDialog::OnMunitionChanged( int nRow, int nCol )
{
    // do only if "munition" field has been changed
    if( nCol != 0 )
        return;
    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( pMunitionsTable_->item( nRow, 0 ) );
    assert( pComboTableItem );

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( nRow != pMunitionsTable_->numRows() - 1 )
        {
            pMunitionsTable_->removeRow( nRow );
            // select last row quantity field
            pMunitionsTable_->setCurrentCell( pMunitionsTable_->numRows() - 1, 1 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( nRow == pMunitionsTable_->numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            int nCurrentItem = pComboTableItem->currentItem();
            uint nPos = nRow + 1;
            pMunitionsTable_->insertRows( nPos, 1 );
            pMunitionsTable_->setItem( nPos, 0, new QComboTableItem( pMunitionsTable_, *pMunitionsStringList_ ) );
            pMunitionsTable_->setText( nPos, 1, "0" );
            // need to set again the combo box selected element
            pComboTableItem->setCurrentItem( nCurrentItem );
        }
        // select quantity field
        pMunitionsTable_->setCurrentCell( nRow, 1 );
    }
}