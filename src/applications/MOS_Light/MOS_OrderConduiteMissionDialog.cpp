//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_OrderConduiteMissionDialog.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 16:11 $
// $Revision: 36 $
// $Workfile: MOS_OrderConduiteMissionDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_OrderConduiteMissionDialog.h"
#include "moc_MOS_OrderConduiteMissionDialog.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_OrderConduiteMissionDialog.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_Agent.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_ASN_Messages.h"

#include "MOS_Mission_ABC.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentListView.h"
#include "MOS_AgentEditor.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_OrderConduiteMissionDialog::MOS_OrderConduiteMissionDialog( QWidget* pParent )
    : QDialog( pParent )
    , pAgent_( 0 )
{
    setCaption( tr("Order mission/conduite") );

    /////////////////////////////////////////
    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );
    
    
    /////////////////////////////////////////
    // First layout
    QGroupBox* pFirstGroupBox = new QGroupBox( "Ordre", this );
    pMainLayout->addWidget( pFirstGroupBox );

    QGridLayout* pFirstLayout = new QGridLayout( pFirstGroupBox, 5, 5 );
    pFirstLayout->setMargin( 20 );

    int nCurRow = 0;

    //----------------------
    // Ordres de conduite
    pFirstLayout->addWidget( new QLabel( tr("Ordre de conduite" ), pFirstGroupBox ), nCurRow, 1 );
    pOrderConduiteComboBox_ = new QComboBox( FALSE, pFirstGroupBox );
    pFirstLayout->addWidget( pOrderConduiteComboBox_, nCurRow, 2 );

    pFirstLayout->addRowSpacing( (++nCurRow)++, 20 );
    
    // Fill combo Box
    FillComboBoxes();

    /////////////////////////////////////////
    // Second layout
    QHBoxLayout* pSecondLayout = new QHBoxLayout( pMainLayout );

    //--------------------
    // OK / Cancel
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pSecondLayout->addWidget( pOKButton );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pSecondLayout->addWidget( pCancelButton );
    pOKButton->setDefault( TRUE );
    
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
    

}

//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_OrderConduiteMissionDialog::~MOS_OrderConduiteMissionDialog()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog::Initialize
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_OrderConduiteMissionDialog::Initialize( MOS_Agent& agent )
{
    assert( !pAgent_ );
    pAgent_ = &agent;
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog::Cleanup
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_OrderConduiteMissionDialog::Cleanup()
{
    pAgent_ = 0;    
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog::FillComboBoxes
// Created: NLD 2003-01-13
//-----------------------------------------------------------------------------
void MOS_OrderConduiteMissionDialog::FillComboBoxes()
{
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_MettreTenueNBC" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_EnleverTenueNBC" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_PasserEnSilenceRadio" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ArreterSilenceRadio" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_PasserEnSilenceRadar" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ArreterSilenceRadar" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_AttendreSePoster" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Poursuivre" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Interrompre" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ChangerReglesEngagement" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Deboucher" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Decrocher" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_AssignerPositionEmbarquement" );    
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_AcquerirObjectif" );    
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Brouiller" );    
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_ChangerDePosition" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_Contourner" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_EnvoyerVehicule" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_ActiverObstacle" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_AppliquerFeux" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_ExecuterTir" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_Emettre" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_StopperEmission" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_RenforcerEnVSRAM" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_TransfererVSRAM" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ModifierTempsBordeeMaintenance" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ModifierTempsBordeeSante" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ModifierPrioritesReparations" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ModifierPrioritesBlesses" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ModifierPrioritesTactiquesReparations" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_ModifierPrioritesTactiquesBlesses" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Automate_RealiserVariantement" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Automate_DesequiperSiteFranchissement" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Automate_ReagirFaceAEni" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Automate_AffecterPionAObstacle" );
    pOrderConduiteComboBox_->insertItem( "eOrdreConduite_Automate_ReconnaitreZoneImplantation" );
}

//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog::SlotValidate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_OrderConduiteMissionDialog::SlotValidate()
{
    assert( pAgent_ );

    E_OrderConduiteID ocId = (E_OrderConduiteID)pOrderConduiteComboBox_->currentItem();
    switch( ocId )
    {
        case eOrdreConduite_ChangerReglesEngagement:
        case eOrdreConduite_Pion_ChangerDePosition:
        case eOrdreConduite_AssignerPositionEmbarquement:
        case eOrdreConduite_AcquerirObjectif:
        case eOrdreConduite_Pion_AppliquerFeux:       
        case eOrdreConduite_Pion_RenforcerEnVSRAM:                   
        case eOrdreConduite_Pion_TransfererVSRAM:                    
        case eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM:            
        case eOrdreConduite_ModifierTempsBordeeMaintenance:                
        case eOrdreConduite_ModifierTempsBordeeSante:                
        case eOrdreConduite_ModifierPrioritesReparations:        
        case eOrdreConduite_ModifierPrioritesBlesses:           
        case eOrdreConduite_ModifierPrioritesTactiquesReparations:        
        case eOrdreConduite_ModifierPrioritesTactiquesBlesses:           
        case eOrdreConduite_Automate_ReconnaitreZoneImplantation :
        case eOrdreConduite_Automate_RealiserVariantement:
        case eOrdreConduite_Automate_DesequiperSiteFranchissement:
        case eOrdreConduite_Automate_ReagirFaceAEni:
        case eOrdreConduite_Automate_AffecterPionAObstacle:
        {
            MOS_Mission_ABC* pOC = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GetConduiteOrder( ocId  );
            assert( pOC != 0 );
            pOC->Initialize();
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pOC );
            pOC->SetAgent( pAgent_ );
            break;
        }
        default:
        {
            MOS_ASN_MsgOrderConduite asnMsg;
            asnMsg.GetAsnMsg().unit_id  = pAgent_->GetAgentID();
            asnMsg.GetAsnMsg().order_id = 43;
            asnMsg.GetAsnMsg().order_conduite.t = pOrderConduiteComboBox_->currentItem() + 1; //$$$ DEGEU pour le moment
            asnMsg.Send( 65 );

            break;
        }
    }
    accept();
  
    Cleanup();
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderConduiteMissionDialog::SlotCancel
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_OrderConduiteMissionDialog::SlotCancel()
{
    Cleanup();
    reject();
}


