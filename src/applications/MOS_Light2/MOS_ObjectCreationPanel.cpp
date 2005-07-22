// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-04 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectCreationPanel.cpp $
// $Author: Age $
// $Modtime: 4/07/05 11:00 $
// $Revision: 17 $
// $Workfile: MOS_ObjectCreationPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_ObjectCreationPanel.h"
#include "moc_MOS_ObjectCreationPanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_ParamLocation.h"
#include "MOS_AgentManager.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_Team.h"
#include "MOS_Tools.h"
#include "MOS_ASN_Messages.h"
#include "MOS_DynaObject.h"
#include "MOS_Object_Def.h"
#include "MOS_Agent.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ObjectCreationPanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
MOS_ObjectCreationPanel::MOS_ObjectCreationPanel( QTabWidget* pParent )
    : QWidget ( pParent )
    , MOS_MapEventFilter_ABC( false )
    , pParent_( pParent )
{
    QGridLayout* pLayout = new QGridLayout( this, 6, 2, 4 );
//    pLayout->setRowSpacing( 3, 8 );
    pLayout->setRowStretch( 5, 1 );
    pLayout->setRowStretch( 2, 0 );

    QLabel* pTeamLabel = new QLabel( tr( "Camp:" ), this );
    pLayout->addWidget( pTeamLabel, 0, 0, Qt::AlignLeft );

    QLabel* pObjectLabel = new QLabel( tr( "Objet:" ), this );
    pLayout->addWidget( pObjectLabel, 1, 0, Qt::AlignLeft );

    QLabel* pLocationLabel = new QLabel( tr( "Emplacement:" ), this );
    pLayout->addWidget( pLocationLabel, 2, 0, Qt::AlignLeft );

    pTeamCombo_ = new MT_ValuedComboBox< uint >( this );
    pLayout->addWidget( pTeamCombo_, 0, 1, Qt::AlignRight );

    pObjectTypeCombo_ = new MT_ValuedComboBox< ASN1T_EnumObjectType >( this );
    pLayout->addWidget( pObjectTypeCombo_, 1, 1, Qt::AlignRight );

    pLocation_ = new MOS_ParamLocation( asnLocation_, "", "Emplacement nouvel objet", this );
    pLayout->addWidget( pLocation_, 2, 1, Qt::AlignRight );

    pNBCParamsGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres NBC" ), this );
    pNBCParamsGroup_->setFlat( true );
    pNBCParamsGroup_->setInsideMargin( 0 );
    pNBCParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pNBCParamsGroup_, 3, 3, 0, 1 );

    new QLabel( tr( "Type:" ), pNBCParamsGroup_ );
    pNBCTypeCombo_ = new MT_ValuedComboBox< uint >( pNBCParamsGroup_ );
    const T_MosId_String_Map& nbcTypes = MOS_App::GetApp().GetNBCNames();
    for( CIT_MosId_String_Map it = nbcTypes.begin(); it != nbcTypes.end(); ++it )
        pNBCTypeCombo_->AddItem( QString( ((*it).second).c_str() ), (*it).first ); 

    pCrossingParamsGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres Franchissement" ), this );
    pCrossingParamsGroup_->setFlat( true );
    pCrossingParamsGroup_->setInsideMargin( 0 );
    pCrossingParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pCrossingParamsGroup_, 4, 4, 0, 1 );

    new QLabel( tr( "Largeur:" ), pCrossingParamsGroup_ );
    pCrossingWidthEdit_ = new QSpinBox( 0, 10000, 1, pCrossingParamsGroup_ );

    new QLabel( tr( "Profondeur:" ), pCrossingParamsGroup_ );
    pCrossingDepthEdit_ = new QSpinBox( 0, 100, 1, pCrossingParamsGroup_ );

    new QLabel( tr( "Vitesse courant:" ), pCrossingParamsGroup_ );
    pCrossingSpeedEdit_ = new QSpinBox( 0, 200, 1, pCrossingParamsGroup_ );

    pCrossingToConstructCheckbox_ = new QCheckBox( tr( "A aménager:" ), pCrossingParamsGroup_ );

    pTC2Group_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres Camp" ), this );
    pCrossingParamsGroup_->setFlat( true );
    pCrossingParamsGroup_->setInsideMargin( 0 );
    pCrossingParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pTC2Group_, 5, 5, 0, 1 );
    new QLabel( tr( "TC2:" ), pTC2Group_ );
    pTC2ComboBox_ = new QComboBox( FALSE, pTC2Group_ );
    pTC2ComboBox_->hide();

    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    pLayout->addWidget( pOkButton, 6, 1, Qt::AlignRight );

    const MOS_AgentManager::T_TeamMap& teamMap = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

    for( int n = 0; n < eNbrObjectTypes; ++n )
        pObjectTypeCombo_->AddItem( MOS_Tools::ToString( (ASN1T_EnumObjectType)n ), (ASN1T_EnumObjectType)n );

    MOS_App::GetApp().processEvents();
    pTeamCombo_->setMinimumWidth( pObjectTypeCombo_->width() );
    pNBCTypeCombo_->setMinimumWidth( pObjectTypeCombo_->width() );

    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pObjectTypeCombo_, SIGNAL( activated( int ) ), this, SLOT( OnObjectChanged() ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this,   SLOT( OnConnexionStatusChanged( bool ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( TeamCreated( MOS_Team& ) ), this,   SLOT( OnTeamCreated( MOS_Team& ) ) );

    this->OnObjectChanged();
    this->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::SetSelectedElement
// Created: APE 2004-10-06
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    //$$$$$ C'est nul.
    selectedElement_ = selectedElement;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel destructor
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
MOS_ObjectCreationPanel::~MOS_ObjectCreationPanel()
{
    // Pop();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-06-07
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( pParent_->currentPage() == this && this->isVisible() )
        pLocation_->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnObjectChanged
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnObjectChanged()
{
    ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)pObjectTypeCombo_->GetValue();
    if( nType == EnumObjectType::rota || nType == EnumObjectType::nuage_nbc || nType == EnumObjectType::zone_nbc )
        pNBCParamsGroup_->show();
    else
        pNBCParamsGroup_->hide();

    if( nType == EnumObjectType::site_franchissement )
        pCrossingParamsGroup_->show();
    else
        pCrossingParamsGroup_->hide();

    if( nType == EnumObjectType::camp_prisonniers
     || nType == EnumObjectType::camp_refugies )
        pTC2Group_->show();
    else
        pTC2Group_->hide();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnOk
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnOk()
{
    if( ! pLocation_->CheckValidity() )
        return;
        
    if( MOS_App::GetApp().IsODBEdition() )
    {
        MOS_DynaObject* pObject = new MOS_DynaObject();
        MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeam( pTeamCombo_->GetValue() );
        assert( pTeam != 0 );
        pObject->SetTeam( *pTeam );
        ASN1T_EnumObjectType nType = pObjectTypeCombo_->GetValue();
        pObject->SetType( nType );

        uint nID =  MOS_DynaObject::GetIDManagerForObjectType( nType ).GetFreeIdentifier();
        pObject->SetID( nID );
        
        pObject->SetLocalisation( pLocation_->GetType(), pLocation_->GetPointList() );
        pLocation_->Clear();
        
        if( nType == EnumObjectType::nuage_nbc )
            pObject->SetNBCParameter( pNBCTypeCombo_->GetValue() );
        
        if( nType == EnumObjectType::site_franchissement )
        {
            pObject->SetFranchissementParameters( pCrossingWidthEdit_->value(), pCrossingDepthEdit_->value(),
                                                  pCrossingSpeedEdit_->value(), pCrossingToConstructCheckbox_->isOn() );
        }
        
        MOS_App::GetApp().GetDynaObjectManager().RegisterDynaObject( *pObject );
        MOS_App::GetApp().NotifyDynaObjectCreated( *pObject );
        return;
    }

    MOS_ASN_MsgObjectMagicAction asnMsg;
    ASN1T_MagicActionCreateObject asnAction;

    asnAction.type = (ASN1T_EnumObjectType)pObjectTypeCombo_->GetValue();
    asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject::GetIDManagerForObjectType( asnAction.type ).GetFreeIdentifier();
    
    asnAction.camp  = pTeamCombo_->GetValue();

    std::stringstream strMsg;
    strMsg << "Demande creation objet " << MOS_Tools::ToString( (ASN1T_EnumObjectType)asnAction.type ) << ". ";

    pLocation_->WriteMsg( strMsg );
    asnAction.localisation = asnLocation_;

    asnMsg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_create_object;
    asnMsg.GetAsnMsg().action.u.create_object   = &asnAction;

    ASN1T_AttrObjectNuageNBC attributsNuageNBC;
    if( asnAction.type == EnumObjectType::nuage_nbc )
    {
        attributsNuageNBC.agent_nbc  = pNBCTypeCombo_->GetValue();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_nuage_nbc;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.nuage_nbc = &attributsNuageNBC;
    }
    ASN1T_AttrObjectZoneNBC  attributsZoneNBC;
    if( asnAction.type == EnumObjectType::zone_nbc )
    {
        attributsZoneNBC.agent_nbc  = pNBCTypeCombo_->GetValue();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_zone_nbc;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.zone_nbc  = &attributsZoneNBC;
    }
    ASN1T_AttrObjectROTA     attributsRota;
    if( asnAction.type == EnumObjectType::rota )
    {
        attributsRota.agents_nbc.n = 1;
        attributsRota.agents_nbc.elem  = new uint[1];
        *attributsRota.agents_nbc.elem = pNBCTypeCombo_->GetValue();
        attributsRota.niveau_danger = 5;
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_rota;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.rota      = &attributsRota;
    }
    ASN1T_AttrObjectSiteFranchissement attributsCrossing;
    if( asnAction.type == EnumObjectType::site_franchissement )
    {
        attributsCrossing.largeur           = pCrossingWidthEdit_->value();
        attributsCrossing.profondeur        = pCrossingDepthEdit_->value();
        attributsCrossing.vitesse_courant   = pCrossingSpeedEdit_->value();
        attributsCrossing.berges_a_amenager = pCrossingToConstructCheckbox_->isOn();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_site_franchissement;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.site_franchissement = &attributsCrossing;
    }
    ASN1T_AttrObjectItineraireLogistique attributsItineraire;
    if( asnAction.type == EnumObjectType::itineraire_logistique )
    {
        // $$$$ AGE 2005-05-02: 
        attributsItineraire.itineraire_equipe = true;
        attributsItineraire.poids_max_supporte = 12;
        attributsItineraire.largeur = 13;
        attributsItineraire.longueur = 14;
        attributsItineraire.debit = 42;
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_itineraire_logistique;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.itineraire_logistique = &attributsItineraire;
    }

    ASN1T_AttrObjectCampPrisonniers attributsCampPrisonniers;
    if( asnAction.type == EnumObjectType::camp_prisonniers )
    {
        attributsCampPrisonniers.tc2 = automateComboBoxIDs_.find( pTC2ComboBox_->currentItem() )->second->GetAgentID();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_camp_prisonniers;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.camp_prisonniers= &attributsCampPrisonniers;
    }

    ASN1T_AttrObjectCampRefugies attributsCampRefugies;
    if( asnAction.type == EnumObjectType::camp_refugies )
    {
        attributsCampRefugies.tc2 = automateComboBoxIDs_.find( pTC2ComboBox_->currentItem() )->second->GetAgentID();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_camp_refugies;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.camp_refugies = &attributsCampRefugies;
    }

    asnMsg.Send( 912 );

    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

    pLocation_->Clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::eventFilter
/** @param  pSender 
    @param  pEvent 
    @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
bool MOS_ObjectCreationPanel::eventFilter( QObject* pSender, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            return OnKeyPress( *(QKeyEvent*)pEvent );
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnKeyPress
/** @param  keyEvent 
    @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
bool MOS_ObjectCreationPanel::OnKeyPress( const QKeyEvent& keyEvent )
{
    switch( keyEvent.key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
        {
            // If we're trying to delete a dynamic object...
            if( selectedElement_.pDynaObject_ != 0 )
            {
                // while editing an odb, we can delete it immediatly.
                if( MOS_App::GetApp().IsODBEdition() )
                {
                    MOS_DynaObject* pObject = selectedElement_.pDynaObject_;
                    MOS_App::GetApp().GetDynaObjectManager().UnregisterDynaObject( *pObject );
                    MOS_App::GetApp().NotifyDynaObjectDeleted( *pObject );
                    delete pObject;
                }
                else  // otherwise, request its deletion.
                {
                    MOS_ASN_MsgObjectMagicAction asnMsg;
                    asnMsg.GetAsnMsg().oid_objet = selectedElement_.pDynaObject_->GetID();
                    asnMsg.GetAsnMsg().action.t  = T_MsgObjectMagicAction_action_destroy_object;
                    asnMsg.Send( 546 );

                    std::stringstream strMsg;
                    strMsg << "Demande destruction objet " << selectedElement_.pDynaObject_->GetID();
                    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
                }
                return true;
            }
        }
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnTeamCreated
// Created: APE 2005-02-24
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnTeamCreated( MOS_Team& team )
{
    pTeamCombo_->AddItem( team.GetName().c_str(), team.GetID() );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnConnexionStatusChanged
// Created: APE 2005-02-24
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnConnexionStatusChanged( bool bConnected )
{
    if( !bConnected )
        pTeamCombo_->Clear();

    // Populate agents;
    automateComboBoxIDs_.clear();
    pTC2ComboBox_->clear();
    MOS_AgentManager::CT_AgentMap& agents = MOS_App::GetApp().GetAgentManager().GetAgentList();
    for( MOS_AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
    {
        const MOS_Agent& agent = *itAgent->second;
        if( !agent.IsAutomate() || !agent.pSupplyData_ )
            continue;

        automateComboBoxIDs_.insert( std::make_pair( pTC2ComboBox_->count(), &agent ) );
        pTC2ComboBox_->insertItem( agent.GetName().c_str() );
    }
}
