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

#include "MOS_Light2_pch.h"
#include "MOS_ObjectCreationPanel.h"
#include "moc_MOS_ObjectCreationPanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_ParamLocation.h"
#include "MOS_ParamAgent.h"
#include "MOS_AgentManager.h"
#include "MOS_ObjectManager.h"
#include "MOS_Team.h"
#include "MOS_Tools.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Object_ABC.h"
#include "MOS_Object_Factory.h"
#include "MOS_Object_Generic.h"
#include "MOS_Object_SiteFranchissement.h"
#include "MOS_Object_NBC.h"
#include "MOS_Object_Camp.h"
#include "MOS_Object_ROTA.h"
#include "MOS_Object_ItineraireLogistique.h"
#include "MOS_Agent.h"
#include "MOS_ActionContext.h"
#include <qtable.h>

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

    //! @name Common part
    //@{
    pLayout->addWidget( new QLabel( tr( "Camp:" ), this ), 0, 0, Qt::AlignLeft );
    pTeamCombo_ = new MT_ValuedComboBox< uint >( this );
    pLayout->addWidget( pTeamCombo_, 0, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Objet:" ), this ), 1, 0, Qt::AlignLeft );
    pObjectTypeCombo_ = new MT_ValuedComboBox< ASN1T_EnumObjectType >( this );
    pLayout->addWidget( pObjectTypeCombo_, 1, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Emplacement:" ), this ), 2, 0, Qt::AlignLeft );
    pLocation_ = new MOS_ParamLocation( asnLocation_, "", "Emplacement nouvel objet", this );
    pLayout->addWidget( pLocation_, 2, 1, Qt::AlignRight );
    //@}

    //! @name NBC parameters
    //@{
    pNBCParamsGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres NBC" ), this );
    pNBCParamsGroup_->setFlat( true );
    pNBCParamsGroup_->setInsideMargin( 0 );
    pNBCParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pNBCParamsGroup_, 3, 3, 0, 1 );

    new QLabel( tr( "Type:" ), pNBCParamsGroup_ );
    pNBCTypeCombo_ = new MT_ValuedComboBox< uint >( pNBCParamsGroup_ );
    const T_MosId_String_Map& nbcTypes = MOS_App::GetApp().GetNBCNames();
    pNBCAgentsStringList_ = new QStringList();
    pNBCAgentsStringList_->push_back( "" );
    for( CIT_MosId_String_Map it = nbcTypes.begin(); it != nbcTypes.end(); ++it )
    {
        pNBCTypeCombo_->AddItem( QString( ((*it).second).c_str() ), (*it).first ); 
        pNBCAgentsStringList_->push_back( QString( it->second.c_str() ) );
    }
    //@}

    //! @name ROTA parameters
    //@{
    pROTAParamsGroup_ = new QGroupBox( 2, Qt::Vertical, tr( "Paramètres ROTA" ), this );
    pROTAParamsGroup_->setFlat( true );
    pROTAParamsGroup_->setInsideMargin( 0 );
    pROTAParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pROTAParamsGroup_, 3, 3, 0, 1 );

    QGroupBox* pDangerGroup = new QGroupBox( 2, Qt::Horizontal, pROTAParamsGroup_ );
    pDangerGroup->setFlat( true );
    pDangerGroup->setInsideMargin( 0 );
    pDangerGroup->setMargin( 0 );
    new QLabel( tr( "Danger:" ), pDangerGroup );
    pROTADangerEdit_ = new QSpinBox( 0, 100, 1, pDangerGroup );

    pROTANBCAgentsTable_ = new QTable( 0, 1, pROTAParamsGroup_ );
    pROTANBCAgentsTable_->horizontalHeader()->setLabel( 0, tr( "Agents NBC" ) );
    pROTANBCAgentsTable_->setLeftMargin( 0 );
    pROTANBCAgentsTable_->setShowGrid( false );

    pROTANBCAgentsTable_->insertRows( 0, 1 );
    pROTANBCAgentsTable_->setItem( 0, 0, new QComboTableItem( pROTANBCAgentsTable_, *pNBCAgentsStringList_ ) );
    //@}

    //! @name Crossing parameters
    //@{
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
    //@}

    //! @name Prisonner/Refugees camp
    //@{
    pTC2Group_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres Camp" ), this );
    pCrossingParamsGroup_->setFlat( true );
    pCrossingParamsGroup_->setInsideMargin( 0 );
    pCrossingParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pTC2Group_, 5, 5, 0, 1 );
	pAgent_ = new MOS_ParamAgent( asnAgent_ , "Tc2: ", "Tc2", pTC2Group_ );
    //@}

    //! @name Logistic route
    //@{
    pLogRouteGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres itinéraire logistique" ), this );
    pLogRouteGroup_->setFlat( true );
    pLogRouteGroup_->setInsideMargin( 0 );
    pLogRouteGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pLogRouteGroup_, 4, 4, 0, 1 );

    new QLabel( tr( "Débit:" )        , pLogRouteGroup_ );
    pLogRouteFlow_      = new QSpinBox( 0, 10000, 1, pLogRouteGroup_ );
    new QLabel( tr( "Largeur:" )      , pLogRouteGroup_ );
    pLogRouteWidth_     = new QSpinBox( 0, 10000, 1, pLogRouteGroup_ );
    new QLabel( tr( "Longueur:" )     , pLogRouteGroup_ );
    pLogRouteLength_    = new QSpinBox( 0, 10000, 1, pLogRouteGroup_ );
    new QLabel( tr( "Poids maximum:" ), pLogRouteGroup_ );
    pLogRouteMaxWeight_ = new QSpinBox( 0, 10000, 1, pLogRouteGroup_ );
    pLogRouteEquipped_  = new QCheckBox( tr( "Equipé:" ), pLogRouteGroup_ );
    //@}

    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    pLayout->addWidget( pOkButton, 6, 1, Qt::AlignRight );

    const MOS_AgentManager::T_TeamMap& teamMap = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

    for( int n = 0; n < eNbrObjectType; ++n )
        pObjectTypeCombo_->AddItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)n ).c_str(), (ASN1T_EnumObjectType)n );


    MOS_App::GetApp().processEvents();
    pTeamCombo_->setMinimumWidth( pObjectTypeCombo_->width() );
    pNBCTypeCombo_->setMinimumWidth( pObjectTypeCombo_->width() );

    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pObjectTypeCombo_   , SIGNAL( activated( int ) ), this, SLOT( OnObjectChanged() ) );
    connect( pROTANBCAgentsTable_, SIGNAL( valueChanged( int, int ) ), SLOT( OnROTANBCAgentChanged( int, int ) ) );
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
	ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)pObjectTypeCombo_->GetValue();
    if( nType == EnumObjectType::camp_refugies || nType == EnumObjectType::camp_prisonniers )
        pAgent_->FillRemotePopupMenu( popupMenu, context );
    if( context.selectedElement_.pObject_ )
    {
        if( popupMenu.count() > 0 )
            popupMenu.insertSeparator();
        popupMenu.insertItem( tr( "Destruction Magique" ), this, SLOT( OnDeleteObject() ) );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnObjectChanged
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnObjectChanged()
{
    ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)pObjectTypeCombo_->GetValue();
    if( nType == EnumObjectType::nuage_nbc || nType == EnumObjectType::zone_nbc )
        pNBCParamsGroup_->show();
    else
        pNBCParamsGroup_->hide();

    if( nType == EnumObjectType::rota )
        pROTAParamsGroup_->show();
    else
        pROTAParamsGroup_->hide();

    if( nType == EnumObjectType::site_franchissement )
        pCrossingParamsGroup_->show();
    else
        pCrossingParamsGroup_->hide();

    if( nType == EnumObjectType::camp_prisonniers || nType == EnumObjectType::camp_refugies )
        pTC2Group_->show();
    else
        pTC2Group_->hide();

    if( nType == EnumObjectType::itineraire_logistique )
        pLogRouteGroup_->show();
    else
        pLogRouteGroup_->hide();
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
        ASN1T_EnumObjectType nType = pObjectTypeCombo_->GetValue();
        MOS_Object_ABC* pObject = MOS_Object_Factory::Create( nType );

        MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeam( pTeamCombo_->GetValue() );
        assert( pTeam != 0 );
        pObject->SetTeam( *pTeam );
        
        uint nID =  MOS_Object_ABC::GetIDManagerForObjectType( nType ).GetFreeIdentifier();
        pObject->SetID( nID );
        
        pObject->SetLocalisation( pLocation_->GetType(), pLocation_->GetPointList() );
        pLocation_->Clear();
		
		if( nType == EnumObjectType::camp_refugies || nType == EnumObjectType::camp_prisonniers )
        {
            static_cast< MOS_Object_Camp* >( pObject )->SetTC2ID( pAgent_->GetAgent()->GetAgentID() );
        }
		pAgent_->Clear();
        
        if( nType == EnumObjectType::nuage_nbc || nType == EnumObjectType::zone_nbc )
        {
            static_cast< MOS_Object_NBC* >( pObject )->SetAgentNbcId( pNBCTypeCombo_->GetValue() );
        }
        
        if( nType == EnumObjectType::rota )
        {
            static_cast< MOS_Object_ROTA* >( pObject )->SetDanger( pROTADangerEdit_->value() );
            for( int i = 0; i < pROTANBCAgentsTable_->numRows() - 1; ++i )
            {
                QComboTableItem* pROTAItem  = static_cast< QComboTableItem* >( pROTANBCAgentsTable_->item( i, 0 ) );
                assert( pROTAItem );
                static_cast< MOS_Object_ROTA* >( pObject )->AddNBCAgent( MOS_App::GetApp().GetNBCID( std::string( pROTAItem->currentText() ) ) );
            }
        }

        if( nType == EnumObjectType::site_franchissement )
        {
            MOS_Object_SiteFranchissement* pObj = static_cast< MOS_Object_SiteFranchissement* >( pObject );
            pObj->SetWidth    ( pCrossingWidthEdit_->value() );
            pObj->SetDepth    ( pCrossingDepthEdit_->value() );
            pObj->SetSpeed    ( pCrossingSpeedEdit_->value() );
            pObj->SetConstruct( pCrossingToConstructCheckbox_->isOn() );
        }
        
        if( nType == EnumObjectType::itineraire_logistique )
        {
            MOS_Object_ItineraireLogistique* pObj = static_cast< MOS_Object_ItineraireLogistique* >( pObject );
            pObj->SetFlow     ( pLogRouteFlow_->value()      );
            pObj->SetWidth    ( pLogRouteWidth_->value()     );
            pObj->SetLength   ( pLogRouteLength_->value()    );
            pObj->SetMaxWeight( pLogRouteMaxWeight_->value() );
            pObj->SetEquipped ( pLogRouteEquipped_->isOn()   );
        }

        MOS_App::GetApp().GetObjectManager().RegisterObject( *pObject );
        MOS_App::GetApp().NotifyObjectCreated( *pObject );
        return;
    }

    MOS_ASN_MsgObjectMagicAction asnMsg;
    ASN1T_MagicActionCreateObject asnAction;

    asnAction.type = (ASN1T_EnumObjectType)pObjectTypeCombo_->GetValue();
    asnMsg.GetAsnMsg().oid_objet = MOS_Object_ABC::GetIDManagerForObjectType( asnAction.type ).GetFreeIdentifier();
    
    asnAction.camp  = pTeamCombo_->GetValue();

    std::stringstream strMsg;
    strMsg << "Demande creation objet " << ENT_Tr::ConvertFromObjectType( (E_ObjectType)asnAction.type ).c_str() << ". ";

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
        attributsRota.agents_nbc.n = pROTANBCAgentsTable_->numRows() - 1;
        attributsRota.agents_nbc.elem  = new uint[ attributsRota.agents_nbc.n ];
        for( int i = 0; i < pROTANBCAgentsTable_->numRows() - 1; ++i )
        {
            QComboTableItem* pROTAItem  = static_cast< QComboTableItem* >( pROTANBCAgentsTable_->item( i, 0 ) );
            assert( pROTAItem );
            attributsRota.agents_nbc.elem[ i ] = MOS_App::GetApp().GetNBCID( std::string( pROTAItem->currentText() ) );
        }
        attributsRota.niveau_danger = pROTADangerEdit_->text().toUInt();
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
        attributsItineraire.itineraire_equipe  = pLogRouteEquipped_->isOn();
        attributsItineraire.poids_max_supporte = pLogRouteMaxWeight_->value();
        attributsItineraire.largeur            = pLogRouteWidth_->value();
        attributsItineraire.longueur           = pLogRouteLength_->value();
        attributsItineraire.debit              = pLogRouteFlow_->value();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_itineraire_logistique;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.itineraire_logistique = &attributsItineraire;
    }

    ASN1T_AttrObjectCampPrisonniers attributsCampPrisonniers;
    if( asnAction.type == EnumObjectType::camp_prisonniers )
    {
        if( ! pAgent_->CheckValidity() )
            return;
		pAgent_->WriteMsg( strMsg );
        attributsCampPrisonniers.tc2 = asnAgent_;
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_camp_prisonniers;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.camp_prisonniers= &attributsCampPrisonniers;
    }

    ASN1T_AttrObjectCampRefugies attributsCampRefugies;
    if( asnAction.type == EnumObjectType::camp_refugies )
    {
        if( ! pAgent_->CheckValidity() )
            return;
        pAgent_->WriteMsg( strMsg );
		attributsCampRefugies.tc2 = asnAgent_;
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_camp_refugies;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.camp_refugies = &attributsCampRefugies;
    }


    asnMsg.Send( 912 );

    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
	pAgent_->Clear();
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
bool MOS_ObjectCreationPanel::eventFilter( QObject*, QEvent* pEvent )
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
    bool bDummy;
    switch( keyEvent.key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
            bDummy = selectedElement_.pObject_;
            OnDeleteObject();
            return bDummy;
    };
    return false;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnDeleteObject
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnDeleteObject()
{
    // If we're trying to delete a dynamic object...
    if( selectedElement_.pObject_ != 0 )
    {
        // while editing an odb, we can delete it immediatly.
        if( MOS_App::GetApp().IsODBEdition() )
        {
            MOS_Object_ABC* pObject = selectedElement_.pObject_;
            MOS_App::GetApp().GetObjectManager().UnregisterObject( *pObject );
            MOS_App::GetApp().NotifyObjectDeleted( *pObject );
            delete pObject;
            selectedElement_.pObject_ = 0;
        }
        else  // otherwise, request its deletion.
        {
            MOS_ASN_MsgObjectMagicAction asnMsg;
            asnMsg.GetAsnMsg().oid_objet = selectedElement_.pObject_->GetID();
            asnMsg.GetAsnMsg().action.t  = T_MsgObjectMagicAction_action_destroy_object;
            asnMsg.Send( 546 );

            std::stringstream strMsg;
            strMsg << "Demande destruction objet " << selectedElement_.pObject_->GetID();
            MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
            selectedElement_.pObject_ = 0;
        }
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnTeamCreated
// Created: APE 2005-02-24
// Modified: SBO 2005-08-03
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnTeamCreated( MOS_Team& team )
{
    pTeamCombo_->Clear();

    const MOS_AgentManager::T_TeamMap& teamMap = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

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
    int i = 0;
    MOS_AgentManager::CT_AgentMap& agents = MOS_App::GetApp().GetAgentManager().GetAgentList();
    for( MOS_AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
    {
        const MOS_Agent& agent = *itAgent->second;
        if( !agent.IsAutomate() || !agent.pSupplyData_ )
            continue;

        automateComboBoxIDs_.insert( std::make_pair( i, &agent ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectCreationPanel::OnROTANBCAgentChanged
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
void MOS_ObjectCreationPanel::OnROTANBCAgentChanged( int nRow, int )
{
    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( pROTANBCAgentsTable_->item( nRow, 0 ) );
    assert( pComboTableItem );

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( nRow != pROTANBCAgentsTable_->numRows() - 1 )
        {
            pROTANBCAgentsTable_->removeRow( nRow );
            // select non existing cell to lose focus
            pROTANBCAgentsTable_->setCurrentCell( 0, 1 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( nRow == pROTANBCAgentsTable_->numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            int nCurrentItem = pComboTableItem->currentItem();
            uint nPos = nRow + 1;
            pROTANBCAgentsTable_->insertRows( nPos, 1 );
            pROTANBCAgentsTable_->setItem( nPos, 0, new QComboTableItem( pROTANBCAgentsTable_, *pNBCAgentsStringList_ ) );
            // need to set again the combo box selected element
            pComboTableItem->setCurrentItem( nCurrentItem );
        }
        // select non existing cell to lose focus
        pROTANBCAgentsTable_->setCurrentCell( 0, 1 );
    }
}
