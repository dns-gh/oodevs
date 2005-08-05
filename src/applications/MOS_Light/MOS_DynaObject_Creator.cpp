// *****************************************************************************
//
// $Created: NLD 2003-08-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_Creator.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:04 $
// $Revision: 33 $
// $Workfile: MOS_DynaObject_Creator.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_DynaObject_Creator.h"
#include "moc_MOS_DynaObject_Creator.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject_Creator.inl"
#endif

#include "MOS_ASN_Messages.h"
#include "MOS_DynaObject.h"
#include "MOS_MainWindow.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_Tools.h"
#include "MOS_AgentManager.h"
#include "MOS_Team.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <qspinbox.h>

MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneNBC_                 = new MOS_IDManager( 126 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerFosseAntiChar_           = new MOS_IDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerAbattis_                 = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerBarricade_               = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerBouchonMines_            = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneMineeLineaire_       = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneMineeParDispersion_  = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerEboulement_              = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerDestructionRoute_        = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerDestructionPont_         = idManagerFosseAntiChar_;// new MIL_MOSIDManager( 147 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPosteTir_                = new MOS_IDManager( 166 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneProtegee_            = idManagerPosteTir_;// new MIL_MOSIDManager( 166 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneImplantationCanon_   = new MOS_IDManager( 167 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneImplantationCOBRA_   = idManagerZoneImplantationCanon_;// new MIL_MOSIDManager( 167 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneImplantationLRM_     = idManagerZoneImplantationCanon_;// new MIL_MOSIDManager( 167 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneImplantationMortier_ = idManagerZoneImplantationCanon_;// new MIL_MOSIDManager( 167 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPontFlottantContinu_     = new MOS_IDManager( 168 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPontFlottantDiscontinu_  = idManagerPontFlottantContinu_;// new MIL_MOSIDManager( 168 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerSiteFranchissement_      = idManagerPontFlottantContinu_;// new MIL_MOSIDManager( 168 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPlotRavitaillement_      = new MOS_IDManager( 189 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerNuageNBC_                = new MOS_IDManager( 190 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerRota_                    = new MOS_IDManager( 197 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerCampPrisonniers_         = new MOS_IDManager( 207 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerCampRefugies_            = idManagerCampPrisonniers_;// new MIL_MOSIDManager( 207 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPosteControle_           = new MOS_IDManager( 208 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPiste_                   = new MOS_IDManager( 209 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerAirePoser_               = new MOS_IDManager( 210 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerPlateForme_              = idManagerAirePoser_;// new MIL_MOSIDManager( 210 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneMobiliteAmelioree_   = idManagerAirePoser_;// new MIL_MOSIDManager( 210 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZonePoserHelicoptere_    = idManagerAirePoser_;// new MIL_MOSIDManager( 210 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerAireLogistique_          = idManagerAirePoser_;// new MIL_MOSIDManager( 210 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerTerrainLargage_          = idManagerAirePoser_;// new MIL_MOSIDManager( 210 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerItineraireLogistique_    = new MOS_IDManager( 211 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneBrouillageBrod_      = new MOS_IDManager( 212 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneBrouillageBromure_   = idManagerZoneBrouillageBrod_;
MOS_IDManager* MOS_DynaObject_Creator::idManagerSiteDecontamination_     = new MOS_IDManager( 213 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneForbiddenFire_       = new MOS_IDManager( 215 );
MOS_IDManager* MOS_DynaObject_Creator::idManagerZoneForbiddenMove_       = idManagerZoneForbiddenFire_;// new MIL_MOSIDManager( 500 );

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Creator constructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MOS_DynaObject_Creator::MOS_DynaObject_Creator( QWidget* pParent )
    : QFrame  ( pParent )
    , pObject_( 0 )
{
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );


    QGridLayout* pFirstLayout = new QGridLayout( pMainLayout );
    pFirstLayout->setMargin( 20 );
    pFirstLayout->setAlignment( Qt::AlignTop );

    int nCurRow = 0;

    // Team
    pFirstLayout->addWidget( new QLabel( "Team", this ) , nCurRow, 0 );
    pTeamComboBox_ = new QComboBox( FALSE, this );

    const MOS_AgentManager::T_TeamIDVector& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamIDVector itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
        pTeamComboBox_->insertItem( (**itTeam).GetName().c_str() );

    pFirstLayout->addWidget( pTeamComboBox_, nCurRow++, 1 );

    // Type
    pFirstLayout->addWidget( new QLabel( "Type", this ) , nCurRow, 0 );
    pTypeComboBox_ = new QComboBox( FALSE, this );
    for( uint i = 0; i < eNbrObjectType; ++i )
        pTypeComboBox_->insertItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)i ).c_str(), i );

    pFirstLayout->addWidget( pTypeComboBox_, nCurRow++, 1 );

    // Localisation
    pLocalisation_ = new MOS_MTH_Localisation( this, "localisation" );
    pLocalisation_->Initialize();
    pFirstLayout->addWidget( pLocalisation_, nCurRow++, 1 );

    //--------------------
    // OK / Cancel

    // Second layout
    QHBoxLayout* pSecondLayout = new QHBoxLayout( pMainLayout );
    
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pSecondLayout->addWidget( pCancelButton );
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pSecondLayout->addWidget( pOKButton );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Creator destructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MOS_DynaObject_Creator::~MOS_DynaObject_Creator()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_Creator::SlotValidate
// Created: NLD 2003-08-05
//-----------------------------------------------------------------------------
void MOS_DynaObject_Creator::SlotValidate()
{
    MOS_ASN_MsgObjectMagicAction asnMsg;
    ASN1T_MagicActionCreateObject asnAction;

    asnAction.type = (ASN1T_EnumObjectType)pTypeComboBox_->currentItem(); 
    switch( asnAction.type )
    {
        case EnumObjectType::bouchon_mines                  : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerBouchonMines_            ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_minee_lineaire            : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneMineeLineaire_       ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_minee_par_dispersion      : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneMineeParDispersion_  ->GetFreeIdentifier(); break;
        case EnumObjectType::fosse_anti_char                : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerFosseAntiChar_           ->GetFreeIdentifier(); break;
        case EnumObjectType::abattis                        : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerAbattis_                 ->GetFreeIdentifier(); break;
        case EnumObjectType::barricade                      : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerBarricade_               ->GetFreeIdentifier(); break;
        case EnumObjectType::eboulement                     : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerEboulement_              ->GetFreeIdentifier(); break;
        case EnumObjectType::destruction_route              : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerDestructionRoute_        ->GetFreeIdentifier(); break;
        case EnumObjectType::destruction_pont               : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerDestructionPont_         ->GetFreeIdentifier(); break;
        case EnumObjectType::pont_flottant_continu          : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPontFlottantContinu_     ->GetFreeIdentifier(); break;
        case EnumObjectType::pont_flottant_discontinu       : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPontFlottantDiscontinu_  ->GetFreeIdentifier(); break;
        case EnumObjectType::poste_tir                      : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPosteTir_                ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_protegee                  : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneProtegee_            ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_implantation_canon        : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneImplantationCanon_   ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_implantation_cobra        : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneImplantationCOBRA_   ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_implantation_lrm          : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneImplantationLRM_     ->GetFreeIdentifier(); break;
        case EnumObjectType::site_franchissement            : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerSiteFranchissement_      ->GetFreeIdentifier(); break;
        case EnumObjectType::nuage_nbc                      : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerNuageNBC_                ->GetFreeIdentifier(); break;
        case EnumObjectType::plot_ravitaillement            : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPlotRavitaillement_      ->GetFreeIdentifier(); break;
        case EnumObjectType::site_decontamination           : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerSiteDecontamination_     ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_brouillage_brod           : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneBrouillageBrod_      ->GetFreeIdentifier(); break;
        case EnumObjectType::rota                           : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerRota_                    ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_nbc                       : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneNBC_                 ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_brouillage_bromure        : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneBrouillageBromure_   ->GetFreeIdentifier(); break;
        case EnumObjectType::aire_poser						: asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerAirePoser_               ->GetFreeIdentifier(); break;
        case EnumObjectType::piste							: asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPiste_                   ->GetFreeIdentifier(); break;
		case EnumObjectType::plateforme					    : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPlateForme_              ->GetFreeIdentifier(); break;
		case EnumObjectType::zone_mobilite_amelioree		: asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneMobiliteAmelioree_   ->GetFreeIdentifier(); break;
		case EnumObjectType::zone_poser_helicoptere  		: asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZonePoserHelicoptere_    ->GetFreeIdentifier(); break;
        case EnumObjectType::aire_logistique                : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerAireLogistique_          ->GetFreeIdentifier(); break;
        case EnumObjectType::itineraire_logistique          : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerItineraireLogistique_    ->GetFreeIdentifier(); break;
        case EnumObjectType::camp_prisonniers               : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerCampPrisonniers_         ->GetFreeIdentifier(); break;
        case EnumObjectType::camp_refugies                  : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerCampRefugies_            ->GetFreeIdentifier(); break;
        case EnumObjectType::poste_controle                 : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerPosteControle_           ->GetFreeIdentifier(); break;
        case EnumObjectType::terrain_largage                : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerTerrainLargage_          ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_interdite_tir             : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneForbiddenFire_       ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_interdite_mouvement       : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneForbiddenMove_       ->GetFreeIdentifier(); break;
        case EnumObjectType::zone_implantation_mortier      : asnMsg.GetAsnMsg().oid_objet = MOS_DynaObject_Creator::idManagerZoneImplantationMortier_ ->GetFreeIdentifier(); break;
        default:
            assert( false );
    }
    
    asnAction.camp  = MOS_App::GetApp().GetAgentManager().GetTeams()[ pTeamComboBox_->currentItem() ]->GetID();

    std::string sParam;
    pLocalisation_->WriteMsg( asnAction.localisation, sParam );
    
    asnMsg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_create_object;
    asnMsg.GetAsnMsg().action.u.create_object   = &asnAction;

    ASN1T_AttrObjectNuageNBC             attributsNBC;
    ASN1T_AttrObjectROTA                 attributsROTA;
    ASN1T_AttrObjectZoneNBC              attributszoneNBC;
    ASN1T_AttrObjectSiteFranchissement   attributsSiteFranchissement;
    ASN1T_AttrObjectItineraireLogistique attributsItineraireLogistique;
    if( asnAction.type == EnumObjectType::nuage_nbc )
    {
        attributsNBC.agent_nbc  = MOS_App::GetApp().GetRandomAgentNBC(); //$$ no comment
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent    = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t           = T_AttrObjectSpecific_nuage_nbc;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.nuage_nbc = &attributsNBC;
    }
    else if( asnAction.type == EnumObjectType::zone_nbc )
    {
        attributszoneNBC.agent_nbc  = MOS_App::GetApp().GetRandomAgentNBC(); //$$ no comment
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent   = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t          = T_AttrObjectSpecific_zone_nbc;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.zone_nbc = &attributszoneNBC;
    }
    else if( asnAction.type == EnumObjectType::rota )
    {
        attributsROTA.niveau_danger   = 2;
        attributsROTA.agents_nbc.n    = 1;
        attributsROTA.agents_nbc.elem = new ASN1T_OID[1];
        attributsROTA.agents_nbc.elem[0] = MOS_App::GetApp().GetRandomAgentNBC();
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent   = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t          = T_AttrObjectSpecific_rota;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.rota     = &attributsROTA;
    }
    else if( asnAction.type == EnumObjectType::site_franchissement )
    {
        attributsSiteFranchissement.berges_a_amenager = false;
        attributsSiteFranchissement.largeur           = 45;
        attributsSiteFranchissement.profondeur        = 8;
        attributsSiteFranchissement.vitesse_courant   = 120;
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent              = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t                     = T_AttrObjectSpecific_site_franchissement;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.site_franchissement = &attributsSiteFranchissement;
    }
    else if( asnAction.type == EnumObjectType::itineraire_logistique )
    {
        attributsItineraireLogistique.itineraire_equipe  = false;
        attributsItineraireLogistique.debit              = 62;
        attributsItineraireLogistique.poids_max_supporte = 12;
        attributsItineraireLogistique.longueur           = 690;
        attributsItineraireLogistique.largeur            = 6;
        asnMsg.GetAsnMsg().action.u.create_object->m.attributs_specifiquesPresent              = 1;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t                     = T_AttrObjectSpecific_itineraire_logistique;
        asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.u.itineraire_logistique = &attributsItineraireLogistique;
    }

    asnMsg.Send( 912 );

    if( asnAction.localisation.vecteur_point.n > 0 )
        delete [] asnAction.localisation.vecteur_point.elem;

    if( asnMsg.GetAsnMsg().action.u.create_object->attributs_specifiques.t == T_AttrObjectSpecific_rota )
        delete [] attributsROTA.agents_nbc.elem;


    pLocalisation_->Clear();
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_Creator::SlotCancel
// Created: NLD 2003-08-05
//-----------------------------------------------------------------------------
void MOS_DynaObject_Creator::SlotCancel()
{
    pLocalisation_->Clear();
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Creator::Initialize
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_DynaObject_Creator::Initialize()
{
    pTeamComboBox_->clear();
    const MOS_AgentManager::T_TeamIDVector& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamIDVector itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
        pTeamComboBox_->insertItem( (**itTeam).GetName().c_str() );    
}

