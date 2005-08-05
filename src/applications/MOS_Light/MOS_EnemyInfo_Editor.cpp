//*****************************************************************************
// 
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Editor.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 15:48 $
// $Revision: 12 $
// $Workfile: MOS_EnemyInfo_Editor.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_EnemyInfo_Editor.h"
#include "moc_MOS_EnemyInfo_Editor.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_EnemyInfo_Editor.inl"
#endif

#include "MOS_EnemyInfo.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_EnemyInfo_Direction.h"
#include "MOS_EnemyInfo_Localisation.h"
#include "MOS_PointListViewItem.h"
#include "MOS_Value.h"
#include "MOS_Team.h"

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor constructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Editor::MOS_EnemyInfo_Editor( QWidget* pParent )
: QFrame( pParent, "Rens Eni" )
, pConfirmationBox_( 0 )
, pHierarchyBox_( 0 )
, pAttitudeBox_( 0 )
, pWeaponBox_( 0 )
, pCategoryBox_( 0 )
, pQualifierBox_( 0 )
, pSpecializationBox_( 0 )
, pMobilityBox_( 0 )
, pDirectionEditor_( 0 )
, pLocationEditor_( 0 )
, pSpeed_( 0 )
, pTeamBox_( 0 )
{
    Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor::Initialize
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Editor::Initialize()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );

    int nCurRow = 0;


    QGroupBox* pFirstGroupBox = new QGroupBox( this );
    pFirstGroupBox->setAlignment( Qt::AlignTop );
    pMainLayout->addWidget( pFirstGroupBox );
    QGridLayout* pFirstLayout = new QGridLayout( pFirstGroupBox );
    pFirstLayout->setMargin( 20 );

    // Confirmation Level
    QLabel* pLabel = new QLabel( tr( "Niveau confirmation" ), pFirstGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pFirstLayout->addWidget( pLabel, nCurRow++, 0 );

    pConfirmationBox_ = new QComboBox( FALSE, pFirstGroupBox );
    pFirstLayout->addWidget( pConfirmationBox_, nCurRow++, 0 );
    pConfirmationBox_->insertItem( "Exact", 0 );
    pConfirmationBox_->insertItem( "Supposé", 1 );
    pConfirmationBox_->insertItem( "Inconnu", 2 );

    // Team
    pLabel = new QLabel( tr( "Camp" ), pFirstGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pFirstLayout->addWidget( pLabel, nCurRow++, 0 );

    pTeamBox_ = new QComboBox( FALSE, pFirstGroupBox );
    pFirstLayout->addWidget( pTeamBox_, nCurRow++, 0 );
    
//    const MOS_AgentManager::T_TeamMap& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
//    for( MOS_AgentManager::CIT_TeamMap itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
//    {
//        pTeamBox_->insertItem( itTeam->second->GetName().c_str(), itTeam->second->GetIdx() );
//    }

//    pTeamBox_->insertItem( "Camp 1", 0 );
//    pTeamBox_->insertItem( "Camp 2", 1 );
//    pTeamBox_->insertItem( "Camp 3", 2 );
//    pTeamBox_->insertItem( "Camp 4", 3 );
//    pTeamBox_->insertItem( "Camp 5", 4 );
//    pTeamBox_->insertItem( "Camp 6", 5 );
//    pTeamBox_->insertItem( "Inconnu", 6 );
//
    // Hierarchical Level
    pLabel = new QLabel( tr( "Niveau Hiérarchique" ), pFirstGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pFirstLayout->addWidget( pLabel, nCurRow++, 0 );
    pHierarchyBox_ = new QComboBox( FALSE, pFirstGroupBox );
    pFirstLayout->addWidget( pHierarchyBox_, nCurRow++, 0 );
    pHierarchyBox_->insertItem( "Groupe",       MOS_EnemyInfo::eHierarchicalLevel_Squad );
    pHierarchyBox_->insertItem( "Section",      MOS_EnemyInfo::eHierarchicalLevel_Section );
    pHierarchyBox_->insertItem( "Equipe",       MOS_EnemyInfo::eHierarchicalLevel_Team );
    pHierarchyBox_->insertItem( "Force alliée", MOS_EnemyInfo::eHierarchicalLevel_AlliedForces );
    pHierarchyBox_->insertItem( "Armée",        MOS_EnemyInfo::eHierarchicalLevel_FieldArmy );
    pHierarchyBox_->insertItem( "Front",        MOS_EnemyInfo::eHierarchicalLevel_ArmyGroup );
    pHierarchyBox_->insertItem( "Division",     MOS_EnemyInfo::eHierarchicalLevel_Division );
    pHierarchyBox_->insertItem( "Corps d'armée",MOS_EnemyInfo::eHierarchicalLevel_Corps );
    pHierarchyBox_->insertItem( "Régiment",     MOS_EnemyInfo::eHierarchicalLevel_Regiment );
    pHierarchyBox_->insertItem( "Brigade",      MOS_EnemyInfo::eHierarchicalLevel_Brigade );
    pHierarchyBox_->insertItem( "Compagnie",    MOS_EnemyInfo::eHierarchicalLevel_Compagny );
    pHierarchyBox_->insertItem( "Bataillon",    MOS_EnemyInfo::eHierarchicalLevel_Battalion );
    pHierarchyBox_->insertItem( "Inconnu",      MOS_EnemyInfo::eHierarchicalLevel_None );

    // Attitude
    pLabel = new QLabel( tr( "Attitude" ), pFirstGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pFirstLayout->addWidget( pLabel, nCurRow++, 0 );
    pAttitudeBox_ = new QComboBox( FALSE, pFirstGroupBox  );
    pFirstLayout->addWidget( pAttitudeBox_, nCurRow++, 0 );
    pAttitudeBox_->insertItem( "Statique",        MOS_EnemyInfo::eAttitude_Static );
    pAttitudeBox_->insertItem( "En Mvt Offensif", MOS_EnemyInfo::eAttitude_OffensiveMove );
    pAttitudeBox_->insertItem( "En Mvt Défensif", MOS_EnemyInfo::eAttitude_DefensiveMove );
    pAttitudeBox_->insertItem( "Inconnue",        MOS_EnemyInfo::eAttitude_NotDefined );

    // Groupe nature
    QGroupBox* pNatureGroupBox = new QGroupBox( "Nature", pFirstGroupBox );
    pFirstLayout->addWidget( pNatureGroupBox, nCurRow++, 0 );
    QGridLayout* pNatureLayout = new QGridLayout( pNatureGroupBox );
    pNatureLayout->setMargin( 20 );
    int nNatureRow = 0;

    // Nature arme
    pLabel = new QLabel( tr( "Arme" ), pNatureGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pNatureLayout->addWidget( pLabel, nNatureRow++, 0 );
    pWeaponBox_ = new QComboBox( FALSE, pNatureGroupBox );
    pNatureLayout->addWidget( pWeaponBox_, nNatureRow++, 0 );
    for( uint eNature = 0; eNature < ( uint )eNbrUnitNatureWeapon; ++eNature )
        pWeaponBox_->insertItem( ENT_Tr::ConvertFromUnitNatureWeapon( ( E_UnitNatureWeapon )eNature ).c_str(), eNature );
    /*
    pWeaponBox_->insertItem( "Anti-char"                                   , eNatureWeapon_Antichar                         );                            
    pWeaponBox_->insertItem( "Administration"                              , eNatureWeapon_Administration                   );                      
    pWeaponBox_->insertItem( "Defense aerienne"                            , eNatureWeapon_DefenseAerienne                  );                     
    pWeaponBox_->insertItem( "Centre de soutien des operations aeriennes"  , eNatureWeapon_CentreSoutienOperationsAeriennes );    
    pWeaponBox_->insertItem( "Blinde"                                      , eNatureWeapon_Blinde                           );                              
    pWeaponBox_->insertItem( "Aviation"                                    , eNatureWeapon_Aviation                         );                            
    pWeaponBox_->insertItem( "Cavalerie"                                   , eNatureWeapon_Cavalerie                        );                           
    pWeaponBox_->insertItem( "Genie"                                       , eNatureWeapon_Genie                            );                               
    pWeaponBox_->insertItem( "Deminage"                                    , eNatureWeapon_Deminage                         );                            
    pWeaponBox_->insertItem( "Soutien artillerie"                          , eNatureWeapon_SoutienArtillerie                );                   
    pWeaponBox_->insertItem( "Artillerie"                                  , eNatureWeapon_Artillerie                       );                          
    pWeaponBox_->insertItem( "Infanterie"                                  , eNatureWeapon_Infanterie                       );                          
    pWeaponBox_->insertItem( "Guerre de l'information"                     , eNatureWeapon_GuerreInformation                );                   
    pWeaponBox_->insertItem( "Forces de securite interne"                  , eNatureWeapon_ForcesSecuriteInterne            );               
    pWeaponBox_->insertItem( "Execution de la loi"                         , eNatureWeapon_ExecutionLoi                     );                        
    pWeaponBox_->insertItem( "Soutien atterrissage"                        , eNatureWeapon_SoutienAtterrissage              );                 
    pWeaponBox_->insertItem( "Logistique"                                  , eNatureWeapon_Logistique                       );                          
    pWeaponBox_->insertItem( "Reparation"                                  , eNatureWeapon_Reparation                       );                          
    pWeaponBox_->insertItem( "Manoeuvre"                                   , eNatureWeapon_Manoeuvre                        );                           
    pWeaponBox_->insertItem( "Medical"                                     , eNatureWeapon_Medical                          );                             
    pWeaponBox_->insertItem( "Renseignement militaire"                     , eNatureWeapon_RenseignementMilitaire           );              
    pWeaponBox_->insertItem( "Missile"                                     , eNatureWeapon_Missile                          );                             
    pWeaponBox_->insertItem( "NBC"                                         , eNatureWeapon_Nbc                              );                                 
    pWeaponBox_->insertItem( "Inconnue"                                    , eNatureWeapon_Inconnue                         );                            
    pWeaponBox_->insertItem( "specifiee"                                   , eNatureWeapon_NonSpecifiee                        );                           
    pWeaponBox_->insertItem( "Reconnaissance"                              , eNatureWeapon_Reconnaissance                   );                      
    pWeaponBox_->insertItem( "Transmissions"                               , eNatureWeapon_Transmissions                    );                       
    pWeaponBox_->insertItem( "Ravitaillement"                              , eNatureWeapon_Ravitaillement                   );                      
    pWeaponBox_->insertItem( "Tactical air control party"                  , eNatureWeapon_TacticalAirControlParty          );             
    pWeaponBox_->insertItem( "Transport"                                   , eNatureWeapon_Transport                        );
    pWeaponBox_->insertItem( "None"                                        , eNatureWeapon_None                             );
    */

    // Nature spécialité
    pLabel = new QLabel( tr( "Specialité" ), pNatureGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pNatureLayout->addWidget( pLabel, nNatureRow++, 0 );
    pSpecializationBox_ = new QComboBox( FALSE, pNatureGroupBox, "Specialité" );
    pNatureLayout->addWidget( pSpecializationBox_, nNatureRow++, 0 );
    for( uint eNature = 0; eNature < ( uint )eNbrUnitNatureSpecialization; ++eNature )
        pSpecializationBox_->insertItem( ENT_Tr::ConvertFromUnitNatureSpecialization( ( E_UnitNatureSpecialization )eNature ).c_str(), eNature );
    /*
    pSpecializationBox_->insertItem(  "Jag"                                      , eNatureSpec_Jag                              );
    pSpecializationBox_->insertItem(  "Riverine"                                 , eNatureSpec_Riverine                         );
    pSpecializationBox_->insertItem(  "Security police"                          , eNatureSpec_SecurityPolice                   );
    pSpecializationBox_->insertItem(  "Aerien"                                   , eNatureSpec_Aerien                           );
    pSpecializationBox_->insertItem(  "Aeroporte"                                , eNatureSpec_Aeroporte                        );
    pSpecializationBox_->insertItem(  "Attaque aerienne"                         , eNatureSpec_AttaqueAerienne                  );
    pSpecializationBox_->insertItem(  "Blinde"                                   , eNatureSpec_Blinde                           );  
    pSpecializationBox_->insertItem(  "Chemin de fer"                            , eNatureSpec_CheminDeFer                      );
    pSpecializationBox_->insertItem(  "Chirurgie"                                , eNatureSpec_Chirurgie                        );
    pSpecializationBox_->insertItem(  "Compose"                                  , eNatureSpec_Compose                          );
    pSpecializationBox_->insertItem(  "Construction"                             , eNatureSpec_Construction                     );
    pSpecializationBox_->insertItem(  "Courrier"                                 , eNatureSpec_Courrier                         );
    pSpecializationBox_->insertItem(  "Decollage - atterissage vertical - court" , eNatureSpec_DecollageAtterissageVerticalCourt);
    pSpecializationBox_->insertItem(  "Dentiste"                                 , eNatureSpec_Dentiste                         );
    pSpecializationBox_->insertItem(  "Finance"                                  , eNatureSpec_Finance                          );
    pSpecializationBox_->insertItem(  "Guerre electronique"                      , eNatureSpec_GuerreElectronique               );
    pSpecializationBox_->insertItem(  "Helicoptere"                              , eNatureSpec_Helicoptere                      );
    pSpecializationBox_->insertItem(  "Mecanise"                                 , eNatureSpec_Mecanise                         );
    pSpecializationBox_->insertItem(  "Motorise"                                 , eNatureSpec_Motorise                         );
    pSpecializationBox_->insertItem(  "Naval"                                    , eNatureSpec_Naval                            );
    pSpecializationBox_->insertItem(  "Patrouille cotiere"                       , eNatureSpec_PatrouilleCotiere                );
    pSpecializationBox_->insertItem(  "Police militaire"                         , eNatureSpec_PoliceMilitaire                  );
    pSpecializationBox_->insertItem(  "Psychologue"                              , eNatureSpec_Psychologique                    );
    pSpecializationBox_->insertItem(  "Religieux"                                , eNatureSpec_Religieux                        );
    pSpecializationBox_->insertItem(  "Service du personnel"                     , eNatureSpec_ServiceDuPersonnel               );
    pSpecializationBox_->insertItem(  "Terrestre"                                , eNatureSpec_Terrestre                        );
    pSpecializationBox_->insertItem(  "Veterinaire"                              , eNatureSpec_Veterinaire                      );
    pSpecializationBox_->insertItem(  "Voilure fixe"                             , eNatureSpec_VoilureFixe                      );
    pSpecializationBox_->insertItem(  "None"                                     , eNatureSpec_None                             );
    */

    // Nature Qualification
    pLabel = new QLabel( tr( "Qualification" ), pNatureGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pNatureLayout->addWidget( pLabel, nNatureRow++, 0 );
    pQualifierBox_ = new QComboBox( FALSE, pNatureGroupBox, "Qualification" );
    pNatureLayout->addWidget( pQualifierBox_, nNatureRow++, 0 );
    for( uint eNature = 0; eNature < ( uint )eNbrUnitNatureQualifier; ++eNature )
        pQualifierBox_->insertItem( ENT_Tr::ConvertFromUnitNatureQualifier( ( E_UnitNatureQualifier )eNature ).c_str(), eNature );
    /*
    pQualifierBox_->insertItem( "Lourd" , eNatureQualifier_Lourd    );
    pQualifierBox_->insertItem( "Leger" , eNatureQualifier_Leger    );
    pQualifierBox_->insertItem( "Medium", eNatureQualifier_Moyen    );
    pQualifierBox_->insertItem( "None"  , eNatureQualifier_None     );
    */

    // Nature Catégorie
    pLabel = new QLabel( tr( "Catégorie" ), pNatureGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pNatureLayout->addWidget( pLabel, nNatureRow++, 0 );
    pCategoryBox_ = new QComboBox( FALSE, pNatureGroupBox, "Catégorie" );
    pNatureLayout->addWidget( pCategoryBox_, nNatureRow++, 0 );
    for( uint eNature = 0; eNature < ( uint )eNbrUnitNatureCategory; ++eNature )
        pCategoryBox_->insertItem( ENT_Tr::ConvertFromUnitNatureCategory( ( E_UnitNatureCategory )eNature ).c_str(), eNature );
    /*
    pCategoryBox_->insertItem( "Combat"            , eNatureCategory_Combat             );
    pCategoryBox_->insertItem( "Soutien logistique", eNatureCategory_SoutienLogistique  ); 
    pCategoryBox_->insertItem( "Combat support"    , eNatureCategory_CombatSupport      );     
    pCategoryBox_->insertItem( "Inconnu"           , eNatureCategory_Inconnu            );           
    pCategoryBox_->insertItem( "None"              , eNatureCategory_None               );              
    */
    
    // Nature Mobilité
    pLabel = new QLabel( tr( "Mobilité" ), pNatureGroupBox );
    pLabel->setAlignment( Qt::AlignTop );
    pNatureLayout->addWidget( pLabel, nNatureRow++, 0 );
    pMobilityBox_ = new QComboBox( FALSE, pNatureGroupBox, "Mobilité" );
    pNatureLayout->addWidget( pMobilityBox_, nNatureRow++, 0 );
    for( uint eNature = 0; eNature < ( uint )eNbrUnitNatureMobility; ++eNature )
        pMobilityBox_->insertItem( ENT_Tr::ConvertFromUnitNatureMobility( ( E_UnitNatureMobility )eNature ).c_str(), eNature );
    /*
    pMobilityBox_->insertItem( "Aeroporte"                  , eNatureMobility_Aeroporte             );
    pMobilityBox_->insertItem( "Aerien compose"             , eNatureMobility_AerienCompose         );
    pMobilityBox_->insertItem( "Aerien voilure fixe"        , eNatureMobility_AerienVoilureFixe     );
    pMobilityBox_->insertItem( "Aerien plus leger que l'air", eNatureMobility_AerienPlusLegerQueAir );
    pMobilityBox_->insertItem( "Aerien ailes rotatives"     , eNatureMobility_AerienAilesRotatives  );
    pMobilityBox_->insertItem( "Aerien amphibie"            , eNatureMobility_AerienAmphibie              );
    pMobilityBox_->insertItem( "Animal monte"               , eNatureMobility_AnimalMonte           );                  
    pMobilityBox_->insertItem( "Demonte"                    , eNatureMobility_Demonte               );
    pMobilityBox_->insertItem( "Terrestre chemin de fer"    , eNatureMobility_TerrestreCheminDeFer  );
    pMobilityBox_->insertItem( "Terrestre auto propulse"    , eNatureMobility_TerrestreAutoPropulse );       
    pMobilityBox_->insertItem( "Terrestre chenille"         , eNatureMobility_TerrestreChenille     );       
    pMobilityBox_->insertItem( "Terrestre remorque"         , eNatureMobility_TerrestreRemorque     );       
    pMobilityBox_->insertItem( "Terrestre roues"            , eNatureMobility_TerrestreRoues        );       
    pMobilityBox_->insertItem( "Maritime sous marin"        , eNatureMobility_MaritimeSousMarin     );       
    pMobilityBox_->insertItem( "Maritime surface"           , eNatureMobility_MaritimeSurface       );       
    pMobilityBox_->insertItem( "Inconnu"                    , eNatureMobility_Inconnu               );       
    pMobilityBox_->insertItem( "None"                       , eNatureMobility_None                  );       
    */

    // Localisation
    pLocationEditor_ = new MOS_EnemyInfo_Localisation( this, "Localisation" );
    pMainLayout->addWidget( pLocationEditor_ );
    pLocationEditor_->Initialize();

    // Direction
    pDirectionEditor_ = new MOS_EnemyInfo_Direction( this, "Direction" );
    pMainLayout->addWidget( pDirectionEditor_ );
    pDirectionEditor_->Initialize();

    // Speed
    pSpeed_ = new MOS_Value( eTypeValueInt, this, "Speed", MOSToolValueModif | MOSToolValueSpeedo | MOSToolValueTitle | MOSToolValueText );
    pSpeed_->setMaximumWidth( 100 );
    pMainLayout->addWidget( pSpeed_ );
    pSpeed_->UpdateValueMin( 0.0 );
    pSpeed_->UpdateValueMax( 100.0 );



    // Second layout
    QHBoxLayout* pSecondLayout = new QHBoxLayout( pMainLayout );

    //--------------------
    // OK / Cancel
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pSecondLayout->addWidget( pCancelButton );
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pSecondLayout->addWidget( pOKButton );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor destructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Editor::~MOS_EnemyInfo_Editor()
{
    delete pDirectionEditor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor::SlotValidate
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Editor::SlotValidate()
{
    assert( pEditedInfo_ != 0 );
    switch( pConfirmationBox_->currentItem() )
    {
    case 0: pEditedInfo_->nConfirmationLevel_ = MOS_EnemyInfo::eConfirmationLevel_Real; break;
    case 1: pEditedInfo_->nConfirmationLevel_ = MOS_EnemyInfo::eConfirmationLevel_Estd; break;
    case 2: pEditedInfo_->nConfirmationLevel_ = MOS_EnemyInfo::eConfirmationLevel_None; break;
    }

    pEditedInfo_->nHierarchicalLevel_ = (MOS_EnemyInfo::E_HierarchicalLevel)pHierarchyBox_->currentItem();
    pEditedInfo_->nAttitude_ = (MOS_EnemyInfo::E_Attitude)pAttitudeBox_->currentItem();
    pEditedInfo_->nWeapon_ = (E_UnitNatureWeapon)pWeaponBox_->currentItem();
    pEditedInfo_->nCategory_ = (E_UnitNatureCategory)pCategoryBox_->currentItem();
    pEditedInfo_->nQualifier_ = (E_UnitNatureQualifier)pQualifierBox_->currentItem();
    pEditedInfo_->nSpecialization_ = (E_UnitNatureSpecialization)pSpecializationBox_->currentItem();
    pEditedInfo_->nMobility_ = (E_UnitNatureMobility)pMobilityBox_->currentItem();

    MOS_PointListViewItem* pStartDir = pDirectionEditor_->GetFirstPoint();
    assert( pStartDir != 0 );
    MOS_PointListViewItem* pEndDir = pDirectionEditor_->GetPointAfter( *pStartDir );
    assert( pEndDir != 0 );

    pEditedInfo_->startDir_ = pStartDir->GetPoint();
    pEditedInfo_->endDir_ = pEndDir->GetPoint();

    pEditedInfo_->nSpeed_ = pSpeed_->GetValueWidget();

    pEditedInfo_->nCamp_ = pTeamBox_->currentItem();

    pEditedInfo_->pointList_.clear();

    MOS_PointListViewItem* pFirstPoint = pLocationEditor_->GetFirstPoint();
    assert( pFirstPoint != 0 );
    while( pFirstPoint != 0 )
    {
        pEditedInfo_->pointList_.push_back( pFirstPoint->GetPoint() );
        pFirstPoint = pLocationEditor_->GetPointAfter( *pFirstPoint );
    }

    pDirectionEditor_->DeleteAllPoints();
    pLocationEditor_->DeleteAllPoints();
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
    pEditedInfo_ = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor::SlotCancel
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Editor::SlotCancel()
{
    pDirectionEditor_->DeleteAllPoints();
    pLocationEditor_->DeleteAllPoints();
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
    pEditedInfo_ = 0;
}



//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor::SetEnemyInfo
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Editor::SetEnemyInfo( MOS_EnemyInfo* pEditedInfo )
{
    assert( pEditedInfo != 0 );

    pEditedInfo_ = pEditedInfo;

    // set the confirmation level
    int nConfirmationItem = 0;
    switch( pEditedInfo->nConfirmationLevel_ )
    {
    case MOS_EnemyInfo::eConfirmationLevel_Real:
        {
            nConfirmationItem = 0;
            break;
        }
    case MOS_EnemyInfo::eConfirmationLevel_Estd:
        {
            nConfirmationItem = 1;
            break;
        }
    case MOS_EnemyInfo::eConfirmationLevel_None:
        {
            nConfirmationItem = 2;
            break;
        }
    }

    pConfirmationBox_->setCurrentItem( nConfirmationItem );

    pTeamBox_->setCurrentItem( pEditedInfo->nCamp_ );

    pHierarchyBox_->setCurrentItem(      pEditedInfo->nHierarchicalLevel_ );
    pAttitudeBox_->setCurrentItem(       pEditedInfo->nAttitude_ );
    pWeaponBox_->setCurrentItem(         pEditedInfo->nWeapon_ );
    pCategoryBox_->setCurrentItem(       pEditedInfo->nCategory_ );
    pQualifierBox_->setCurrentItem(      pEditedInfo->nQualifier_ );
    pSpecializationBox_->setCurrentItem( pEditedInfo->nSpecialization_ );
    pMobilityBox_->setCurrentItem(       pEditedInfo->nMobility_ );

    MOS_PointListViewItem& startItem = pDirectionEditor_->CreatePoint( pEditedInfo->startDir_ );
    pDirectionEditor_->CreatePoint( pEditedInfo->endDir_, & startItem );

    pSpeed_->UpdateValue( pEditedInfo_->nSpeed_ );

    assert( ! pEditedInfo_->pointList_.empty() );
    MOS_PointListViewItem* pFirstItem = 0;
    for( IT_PointVector it = pEditedInfo_->pointList_.begin(); it != pEditedInfo_->pointList_.end(); ++it )
    {
        pFirstItem = & pLocationEditor_->CreatePoint( (*it), pFirstItem );
    }
}

