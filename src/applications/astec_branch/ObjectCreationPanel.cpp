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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectCreationPanel.cpp $
// $Author: Age $
// $Modtime: 4/07/05 11:00 $
// $Revision: 17 $
// $Workfile: ObjectCreationPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ObjectCreationPanel.h"
#include "moc_ObjectCreationPanel.cpp"

#include "App.h"
#include "MainWindow.h"
#include "ParamLocation.h"
#include "ParamAgent.h"
#include "AgentManager.h"
#include "ObjectManager.h"
#include "Team.h"
#include "Tools.h"
#include "ASN_Messages.h"
#include "Object_ABC.h"
#include "Object_Factory.h"
#include "Object_Generic.h"
#include "Object_SiteFranchissement.h"
#include "Object_NBC.h"
#include "Object_Camp.h"
#include "Object_ROTA.h"
#include "Object_ItineraireLogistique.h"
#include "Agent.h"
#include "ActionContext.h"
#include "Model.h"
#include <qtable.h>

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QTabWidget* pParent )
    : QWidget ( pParent )
    , MapEventFilter_ABC( false )
    , pParent_( pParent )
{
    QGridLayout* pLayout = new QGridLayout( this, 7, 2, 4 );
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

    pLayout->addWidget( new QLabel( tr( "Nom:" ), this ), 2, 0, Qt::AlignLeft );
    pNameEdit_ = new QLineEdit( this );
    pLayout->addWidget( pNameEdit_, 2, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Emplacement:" ), this ), 3, 0, Qt::AlignLeft );
    pLocation_ = new ParamLocation( asnLocation_, "", "Emplacement nouvel objet", this, false );
    pLayout->addWidget( pLocation_, 3, 1, Qt::AlignRight );
    //@}

    //! @name NBC parameters
    //@{
    pNBCParamsGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres NBC" ), this );
    pNBCParamsGroup_->setFlat( true );
    pNBCParamsGroup_->setInsideMargin( 0 );
    pNBCParamsGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pNBCParamsGroup_, 4, 4, 0, 1 );

    new QLabel( tr( "Type:" ), pNBCParamsGroup_ );
    pNBCTypeCombo_ = new MT_ValuedComboBox< uint >( pNBCParamsGroup_ );
    const T_MosId_String_Map& nbcTypes = App::GetApp().GetNBCNames();
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
    pLayout->addMultiCellWidget( pROTAParamsGroup_, 4, 4, 0, 1 );

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
    pLayout->addMultiCellWidget( pCrossingParamsGroup_, 5, 5, 0, 1 );

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
    pLayout->addMultiCellWidget( pTC2Group_, 6, 6, 0, 1 );
	pAgent_ = new ParamAgent( asnAgent_ , "Tc2: ", "Tc2", pTC2Group_, false );
    //@}

    //! @name Logistic route
    //@{
    pLogRouteGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres itinéraire logistique" ), this );
    pLogRouteGroup_->setFlat( true );
    pLogRouteGroup_->setInsideMargin( 0 );
    pLogRouteGroup_->setMargin( 0 );
    pLayout->addMultiCellWidget( pLogRouteGroup_, 5, 5, 0, 1 );

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

    // sorted object list
    pObjectTypeCombo_->setSorting( true );
    for( uint n = 0; n < eNbrObjectType; ++n )
        pObjectTypeCombo_->AddItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)n ).c_str(), (ASN1T_EnumObjectType)n );
    

    App::GetApp().processEvents();
    pTeamCombo_->setMinimumWidth( pObjectTypeCombo_->width() );
    pNBCTypeCombo_->setMinimumWidth( pObjectTypeCombo_->width() );

    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pObjectTypeCombo_   , SIGNAL( activated( int ) ), this, SLOT( OnObjectChanged() ) );
    connect( pROTANBCAgentsTable_, SIGNAL( valueChanged( int, int ) ), SLOT( OnROTANBCAgentChanged( int, int ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this,   SLOT( OnConnexionStatusChanged( bool ) ) );
    connect( &App::GetApp(), SIGNAL( TeamCreated( Team& ) ), this,   SLOT( OnTeamCreated( Team& ) ) );

    this->OnObjectChanged();
    this->Push();

}


// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::SetSelectedElement
// Created: APE 2004-10-06
// -----------------------------------------------------------------------------
void ObjectCreationPanel::SetSelectedElement( SelectedElement& selectedElement )
{
    //$$$$$ C'est nul.
    selectedElement_ = selectedElement;
}


// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel destructor
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
ObjectCreationPanel::~ObjectCreationPanel()
{
    // Pop();
}


// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::FillRemotePopupMenu
// Created: APE 2004-06-07
// -----------------------------------------------------------------------------
void ObjectCreationPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
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
// Name: ObjectCreationPanel::OnObjectChanged
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void ObjectCreationPanel::OnObjectChanged()
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
// Name: ObjectCreationPanel::OnOk
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
void ObjectCreationPanel::OnOk()
{
    if( ! pLocation_->CheckValidity() )
        return;
    if( ! App::GetApp().GetNetwork().IsConnected()
     && ! App::GetApp().IsODBEdition() )
        return;
        
    if( App::GetApp().IsODBEdition() )
    {
        ASN1T_EnumObjectType nType = pObjectTypeCombo_->GetValue();
        Object_ABC* pObject = Object_Factory::Create( nType );

        Team* pTeam = & App::GetApp().GetModel().GetTeam( pTeamCombo_->GetValue() );
        pObject->SetTeam( *pTeam );
        
        uint nID =  Object_ABC::GetIDManagerForObjectType( nType ).GetFreeIdentifier();
        pObject->SetID( nID );

        if( !pNameEdit_->text().isEmpty() )
            pObject->SetName( pNameEdit_->text().ascii() );
        
        pObject->SetLocalisation( pLocation_->GetType(), pLocation_->GetPointList() );
        pLocation_->Clear();
		
		if( nType == EnumObjectType::camp_refugies || nType == EnumObjectType::camp_prisonniers )
        {
            static_cast< Object_Camp* >( pObject )->SetTC2ID( pAgent_->GetAgent()->GetID() );
        }
		pAgent_->Clear();
        
        if( nType == EnumObjectType::nuage_nbc || nType == EnumObjectType::zone_nbc )
        {
            static_cast< Object_NBC* >( pObject )->SetAgentNbcId( pNBCTypeCombo_->GetValue() );
        }
        
        if( nType == EnumObjectType::rota )
        {
            static_cast< Object_ROTA* >( pObject )->SetDanger( pROTADangerEdit_->value() );
            for( int i = 0; i < pROTANBCAgentsTable_->numRows() - 1; ++i )
            {
                QComboTableItem* pROTAItem  = static_cast< QComboTableItem* >( pROTANBCAgentsTable_->item( i, 0 ) );
                assert( pROTAItem );
                static_cast< Object_ROTA* >( pObject )->AddNBCAgent( App::GetApp().GetNBCID( std::string( pROTAItem->currentText() ) ) );
            }
        }

        if( nType == EnumObjectType::site_franchissement )
        {
            Object_SiteFranchissement* pObj = static_cast< Object_SiteFranchissement* >( pObject );
            pObj->SetWidth    ( pCrossingWidthEdit_->value() );
            pObj->SetDepth    ( pCrossingDepthEdit_->value() );
            pObj->SetSpeed    ( pCrossingSpeedEdit_->value() );
            pObj->SetConstruct( pCrossingToConstructCheckbox_->isOn() );
        }
        
        if( nType == EnumObjectType::itineraire_logistique )
        {
            Object_ItineraireLogistique* pObj = static_cast< Object_ItineraireLogistique* >( pObject );
            pObj->SetFlow     ( pLogRouteFlow_->value()      );
            pObj->SetWidth    ( pLogRouteWidth_->value()     );
            pObj->SetLength   ( pLogRouteLength_->value()    );
            pObj->SetMaxWeight( pLogRouteMaxWeight_->value() );
            pObj->SetEquipped ( pLogRouteEquipped_->isOn()   );
        }

        App::GetApp().GetObjectManager().RegisterObject( *pObject );
        App::GetApp().NotifyObjectCreated( *pObject );
        return;
    }

    ASN_MsgObjectMagicAction asnMsg;
    ASN1T_MagicActionCreateObject asnAction;

    asnAction.type = (ASN1T_EnumObjectType)pObjectTypeCombo_->GetValue();
    asnMsg.GetAsnMsg().oid_objet = Object_ABC::GetIDManagerForObjectType( asnAction.type ).GetFreeIdentifier();
    
    asnAction.camp  = pTeamCombo_->GetValue();

    std::stringstream strMsg;
    strMsg << "Demande creation objet " << ENT_Tr::ConvertFromObjectType( (E_ObjectType)asnAction.type ).c_str() << ". ";

    pLocation_->WriteMsg( strMsg );
    asnAction.localisation = asnLocation_;

    asnMsg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_create_object;
    asnMsg.GetAsnMsg().action.u.create_object   = &asnAction;

    if( !pNameEdit_->text().isEmpty() )
    {
        asnMsg.GetAsnMsg().action.u.create_object->m.nomPresent = 1;
        asnMsg.GetAsnMsg().action.u.create_object->nom = pNameEdit_->text().ascii();
    }

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
            attributsRota.agents_nbc.elem[ i ] = App::GetApp().GetNBCID( std::string( pROTAItem->currentText() ) );
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
// Name: ObjectCreationPanel::eventFilter
/** @param  pSender 
    @param  pEvent 
    @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
bool ObjectCreationPanel::eventFilter( QObject*, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            return OnKeyPress( *(QKeyEvent*)pEvent );
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::OnKeyPress
/** @param  keyEvent 
    @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
bool ObjectCreationPanel::OnKeyPress( const QKeyEvent& keyEvent )
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
// Name: ObjectCreationPanel::OnDeleteObject
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ObjectCreationPanel::OnDeleteObject()
{
    // If we're trying to delete a dynamic object...
    if( selectedElement_.pObject_ != 0 )
    {
        // while editing an odb, we can delete it immediatly.
        if( App::GetApp().IsODBEdition() )
        {
            Object_ABC* pObject = selectedElement_.pObject_;
            App::GetApp().GetObjectManager().UnregisterObject( *pObject );
            App::GetApp().NotifyObjectDeleted( *pObject );
            delete pObject;
            selectedElement_.pObject_ = 0;
        }
        else  // otherwise, request its deletion.
        {
            ASN_MsgObjectMagicAction asnMsg;
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
// Name: ObjectCreationPanel::OnTeamCreated
// Created: APE 2005-02-24
// Modified: SBO 2005-08-03
// -----------------------------------------------------------------------------
void ObjectCreationPanel::OnTeamCreated( Team& /*team*/ )
{
    pTeamCombo_->Clear();
}

namespace
{
    template< typename Container >
    struct AgentInserter
    {
        AgentInserter( Container& cont ) : cont_( cont ), i_( 0 ) {}
        void operator()( Agent& agent ) const
        {
            if( agent.IsAutomate() && agent.pSupplyData_ )
                cont_.insert( std::make_pair( i_++, &agent ) );
        }
        mutable Container& cont_;
        mutable int i_;
    private:
        AgentInserter& operator=( const AgentInserter& );
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::OnConnexionStatusChanged
// Created: APE 2005-02-24
// -----------------------------------------------------------------------------
void ObjectCreationPanel::OnConnexionStatusChanged( bool bConnected )
{
    if( !bConnected )
        pTeamCombo_->Clear();

    // Populate agents;
    automateComboBoxIDs_.clear();
    App::GetApp().GetModel().ApplyOnAgents( AgentInserter< T_AgentIDMap >( automateComboBoxIDs_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::OnROTANBCAgentChanged
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
void ObjectCreationPanel::OnROTANBCAgentChanged( int nRow, int )
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
