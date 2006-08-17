// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Object.cpp $
// $Author: Nld $
// $Modtime: 9/06/05 18:43 $
// $Revision: 12 $
// $Workfile: DEC_Knowledge_Object.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_Object.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/RC/MIL_RC_ObjetDetecte.h"
#include "Entities/MIL_Army.h"
#include "Tools/MIL_MOSIDManager.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_Object, "DEC_Knowledge_Object" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_ABC                 ()
    , pArmyKnowing_                     ( &armyKnowing )
    , pObjectKnown_                     ( &objectKnown )
    , pObjectType_                      ( &objectKnown.GetType() )
    , nID_                              ( pObjectType_->GetIDManager().GetFreeSimID() )
    , nAttributesUpdated_               ( eAttr_AllAttributes )
    , pOwnerArmy_                       ( &objectKnown.GetArmy() )    
    , localisation_                     ( )
    , avoidanceLocalisation_            ( )
    , nConstructionPercentage_          ( 0 )
    , nMiningPercentage_                ( 0 )
    , nBypassPercentage_                ( 0 )
    , bIsPrepared_                      ( objectKnown.IsPrepared() ) //$$$ A CHIER ....
    , nNbrDotationForConstruction_      ( 0 )
    , nNbrDotationForMining_            ( 0 )                
    , pCurrentPerceptionLevel_          ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_         ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_              ( &PHY_PerceptionLevel::notSeen_ )
    , perceptionPerAutomateSet_         ()
    , previousPerceptionPerAutomateSet_ ()
    , nTimeLastUpdate_                  ( 0 )
    , rRelevance_                       ( 1. )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object()
    : DEC_Knowledge_ABC                ()
    , pObjectType_                     ( 0 )
    , pArmyKnowing_                    ( 0 )
    , pObjectKnown_                    ( 0 )
    , nID_                             ( 0 )
    , nAttributesUpdated_              ( 0 )
    , pOwnerArmy_                      ( 0 )
    , localisation_                    ()
    , avoidanceLocalisation_           ()
    , nConstructionPercentage_         ( 0 )
    , nMiningPercentage_               ( 0 )
    , nBypassPercentage_               ( 0 )
    , bIsPrepared_                     ( false )
    , nNbrDotationForConstruction_     ( 0 )
    , nNbrDotationForMining_           ( 0 )            
    , pCurrentPerceptionLevel_         ( 0 )
    , pPreviousPerceptionLevel_        ( 0 )
    , pMaxPerceptionLevel_             ( 0 )
    , perceptionPerAutomateSet_        ()
    , previousPerceptionPerAutomateSet_()
    , nTimeLastUpdate_                 ( 0 )
    , rRelevance_                      ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::~DEC_Knowledge_Object()
{
    assert( pObjectType_ );
    
    SendMsgDestruction();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::load( MIL_CheckPointInArchive& file, const uint )
{
    ASN1T_EnumObjectType nObjectTypeID;
    
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this ); 
    
    file >> nObjectTypeID;
    pObjectType_ = MIL_RealObjectType::FindObjectType( nObjectTypeID );
    assert( pObjectType_ );
    
    file >> const_cast< MIL_Army*& >( pArmyKnowing_ )
         >> pObjectKnown_
         >> const_cast< uint& >( nID_ )
         >> nAttributesUpdated_
         >> const_cast< MIL_Army*& >( pOwnerArmy_ )
         >> localisation_
         >> avoidanceLocalisation_
         >> nConstructionPercentage_
         >> nMiningPercentage_
         >> nBypassPercentage_
         >> bIsPrepared_
         >> nNbrDotationForConstruction_
         >> nNbrDotationForMining_;
         
    uint nPerceptionID;
    file >> nPerceptionID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nPerceptionID );
    
    file >> nPerceptionID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nPerceptionID );
    
    file >> nPerceptionID;
    pMaxPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nPerceptionID );

    file >> perceptionPerAutomateSet_
         >> previousPerceptionPerAutomateSet_
         >> nTimeLastUpdate_
         >> rRelevance_;
    
    // récupération des noms des types
    uint nSize;
    file >> nSize;
    while ( nSize-- )
    {
        uint nID;
        file >> nID;
        reconByAgentTypes_.insert( MIL_AgentTypePion::FindPionType( nID ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pObjectType_ );
    
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << pObjectType_->GetAsnID()
         << pArmyKnowing_
         << pObjectKnown_
         << nID_
         << nAttributesUpdated_
         << pOwnerArmy_
         << localisation_
         << avoidanceLocalisation_
         << nConstructionPercentage_
         << nMiningPercentage_
         << nBypassPercentage_
         << bIsPrepared_
         << nNbrDotationForConstruction_
         << nNbrDotationForMining_
         << pCurrentPerceptionLevel_->GetID()
         << pPreviousPerceptionLevel_->GetID()
         << pMaxPerceptionLevel_->GetID()
         << perceptionPerAutomateSet_
         << previousPerceptionPerAutomateSet_
         << nTimeLastUpdate_
         << rRelevance_;
    
    // On stocke les types selon leur nom
    file << reconByAgentTypes_.size();
    for ( CIT_AgentTypeSet it = reconByAgentTypes_.begin(); it != reconByAgentTypes_.end(); ++it )
        file << (*it)->GetID();
}


// =============================================================================
// INTERNAL UPDATERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Prepare()
{
    nAttributesUpdated_          = eAttr_Nothing;
    pPreviousPerceptionLevel_    = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_     = &PHY_PerceptionLevel::notSeen_;   

    previousPerceptionPerAutomateSet_.clear();
    perceptionPerAutomateSet_.swap( previousPerceptionPerAutomateSet_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateLocalisations
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdateLocalisations()
{
    if( !pObjectKnown_ )
        return;

    const TER_Localisation& localisation          = pObjectKnown_->GetLocalisation();
    const TER_Localisation& avoidanceLocalisation = pObjectKnown_->GetAvoidanceLocalisation();

    if( !(localisation_ == localisation) )
    {
        localisation_.Reset( localisation );
        NotifyAttributeUpdated( eAttr_Localisation );
    }

    if( !(avoidanceLocalisation_ == avoidanceLocalisation) )
    {
        avoidanceLocalisation_.Reset( avoidanceLocalisation );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateStates
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdateStates()
{
    assert( pObjectKnown_ && !pObjectKnown_->IsMarkedForDestruction() );
    
    const uint nNewConstructionPercentage = (uint)( pObjectKnown_->GetConstructionPercentage() * 100. );
    if( nNewConstructionPercentage != nConstructionPercentage_ )
    {
        nConstructionPercentage_ = nNewConstructionPercentage;
        NotifyAttributeUpdated( eAttr_ConstructionPercentage );
    }

    const uint nNewMiningPercentage = (uint)( pObjectKnown_->GetMiningPercentage() * 100. );
    if( nNewMiningPercentage != nMiningPercentage_ )
    {
        nMiningPercentage_ = nNewMiningPercentage;
        NotifyAttributeUpdated( eAttr_MiningPercentage );
    }

    const uint nNewBypassPercentage = (uint)( pObjectKnown_->GetBypassPercentage() * 100. );
    if( nNewBypassPercentage != nBypassPercentage_ )
    {
        nBypassPercentage_ = nNewBypassPercentage;
        NotifyAttributeUpdated( eAttr_BypassPercentage );
    }

    if( pObjectKnown_->IsPrepared() != bIsPrepared_ )
    {
        bIsPrepared_ = pObjectKnown_->IsPrepared();
        NotifyAttributeUpdated( eAttr_IsPrepared );
    }

    if( pObjectKnown_->GetNbrDotationForConstruction() != nNbrDotationForConstruction_ )
    {
        nNbrDotationForConstruction_ = pObjectKnown_->GetNbrDotationForConstruction();
        NotifyAttributeUpdated( eAttr_Dotations );
    }

    if( pObjectKnown_->GetNbrDotationForMining() != nNbrDotationForMining_ )
    {
        nNbrDotationForMining_ = pObjectKnown_->GetNbrDotationForMining();
        NotifyAttributeUpdated( eAttr_Dotations );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdatePerceptionSources
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdatePerceptionSources( const DEC_Knowledge_ObjectPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;

    MIL_Automate* pAutomateSource = const_cast< MIL_Automate* >( &perception.GetAgentPerceiving().GetAutomate() );   

    // On ne garde que les sources provenant d'autres GTIAs
    IT_PerceptionSourceSet it = perceptionPerAutomateSet_.find( pAutomateSource );
    if( it == perceptionPerAutomateSet_.end() )
        perceptionPerAutomateSet_.insert( pAutomateSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdateCurrentPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateMaxPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::UpdateMaxPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel > *pMaxPerceptionLevel_ )
    {
        pMaxPerceptionLevel_ = &perceptionLevel;
        NotifyAttributeUpdated( eAttr_MaxPerceptionLevel );
        return true;
    }    
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Update
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Update( const PHY_PerceptionLevel& currentPerceptionLevel )
{
    nTimeLastUpdate_ = GetCurrentTimeStep();
    
    UpdateCurrentPerceptionLevel( currentPerceptionLevel );
    UpdateMaxPerceptionLevel    ( currentPerceptionLevel );

    // NB - Quand nPerceptionLevel vaut eNotPerceived => l'agent associé vient juste d'être perdu de vue
    //      => Pas de eNotPerceived aux ticks suivant la perte de contact
    UpdateLocalisations     ();  // Updaté même quand 'NotPerceived', pour les objets pouvant bouger
    UpdateSpecificAttributes( currentPerceptionLevel );
   
    if( currentPerceptionLevel > PHY_PerceptionLevel::notSeen_ )
        UpdateStates();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Update
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Update( const DEC_Knowledge_ObjectPerception& perception )
{
    nTimeLastUpdate_ = GetCurrentTimeStep();

    const PHY_PerceptionLevel&  currentPerceptionLevel = perception.GetCurrentPerceptionLevel();
    
    UpdateCurrentPerceptionLevel( currentPerceptionLevel );
    if( UpdateMaxPerceptionLevel( currentPerceptionLevel ) )
        MIL_RC::pRcObjetDetecte_->Send( perception.GetAgentPerceiving(), MIL_RC::eRcTypeOperational, *this );

    // NB - Quand nPerceptionLevel vaut eNotPerceived => l'agent associé vient juste d'être perdu de vue
    //      => Pas de eNotPerceived aux ticks suivant la perte de contact
    UpdateLocalisations     ();  // Updaté même quand 'NotPerceived', pour les objets pouvant bouger
    UpdateSpecificAttributes( perception );
    UpdatePerceptionSources ( perception );
   
    if( currentPerceptionLevel > PHY_PerceptionLevel::notSeen_ )
        UpdateStates();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Update
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Update( const DEC_Knowledge_ObjectCollision& collision )
{
    if( !collision.IsValid() )
        return;

    assert( pObjectKnown_ );
    nTimeLastUpdate_ = GetCurrentTimeStep();
    
    UpdateCurrentPerceptionLevel( PHY_PerceptionLevel::identified_ );
    UpdateMaxPerceptionLevel    ( PHY_PerceptionLevel::identified_ );
    UpdateStates                ();
 
    //$$$ TMP
    if( !(localisation_ == pObjectKnown_->GetLocalisation() ) )
    {
        if(     localisation_.GetType() != TER_Localisation::ePoint
            ||  localisation_.GetPoints()[ 0 ] != collision.GetPosition() )
        {
            localisation_.Reset( collision.GetPosition() );
            NotifyAttributeUpdated( eAttr_Localisation );
        }
    }
    UpdateSpecificAttributes( collision );
}

// =============================================================================
// RELEVANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateRelevance()
{ 
    assert( rRelevance_ > 0. );

    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( pObjectKnown_ && !pObjectKnown_->IsMarkedForDestruction() );
        if( rRelevance_ != 1. )
        {
            rRelevance_ = 1.;
            NotifyAttributeUpdated( eAttr_Relevance );
        }
        return;
    }

    // L'objet réel va être détruit
    if( pObjectKnown_ && pObjectKnown_->IsMarkedForDestruction() )
    {
        pObjectKnown_ = 0;
        NotifyAttributeUpdated( eAttr_RealObject );
    }

    // Si plus d'objet réel associé est si l'emplacement de l'objet est vu
    assert( pArmyKnowing_ );
    if ( !pObjectKnown_ && pArmyKnowing_->IsPerceived( *this ) )
    {
        rRelevance_ = 0.;
        NotifyAttributeUpdated( eAttr_Relevance );
        return;
    }   
    nTimeLastUpdate_ = GetCurrentTimeStep();
}

// =============================================================================
// NETWORK UPDATERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgPerceptionSources
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgPerceptionSources( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const
{
    if( !IsAttributeUpdated( eAttr_PerceptionSources ) )
        return;

    asnMsg.m.perception_par_compagniePresent = 1;
    asnMsg.perception_par_compagnie.n        = perceptionPerAutomateSet_.size();

    if( perceptionPerAutomateSet_.empty() )
        return;

    ASN1T_OID* pPerceptions = new ASN1T_OID[ perceptionPerAutomateSet_.size() ]; //$$ RAM
    uint i = 0;
    for( CIT_PerceptionSourceSet it = perceptionPerAutomateSet_.begin(); it != perceptionPerAutomateSet_.end(); ++it )
    {
        pPerceptions[i] = (*it)->GetID();
        ++i;
    }
    asnMsg.perception_par_compagnie.elem = pPerceptions;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRelevance
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgRelevance( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const
{
    if( !IsAttributeUpdated( eAttr_Relevance ) )
        return;

    asnMsg.m.pertinencePresent = 1;
    asnMsg.pertinence = (uint)( rRelevance_ * 100. );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgLocalisations
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgLocalisations( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const
{
    if( !IsAttributeUpdated( eAttr_Localisation ) )
        return;

    asnMsg.m.localisationPresent = 1;
    NET_ASN_Tools::WriteLocation( localisation_, asnMsg.localisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const
{
    if( !IsAttributeUpdated( eAttr_CurrentPerceptionLevel ) )
        return;

    asnMsg.m.est_percuPresent = 1;
    asnMsg.est_percu = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgStates
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgStates( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const
{
    assert( pObjectType_ );
    if( *pMaxPerceptionLevel_ == PHY_PerceptionLevel::notSeen_ )
        return;

    if( IsAttributeUpdated( eAttr_ConstructionPercentage ) )
    {
        asnMsg.m.pourcentage_constructionPresent = 1;
        asnMsg.pourcentage_construction = nConstructionPercentage_;
    }

    if( IsAttributeUpdated( eAttr_MiningPercentage ) )
    {
        asnMsg.m.pourcentage_valorisationPresent = 1;
        asnMsg.pourcentage_valorisation = nMiningPercentage_;
    }

    if( IsAttributeUpdated( eAttr_BypassPercentage ) )
    {
        asnMsg.m.pourcentage_contournementPresent = 1;
        asnMsg.pourcentage_contournement = nBypassPercentage_;
    }

    if( IsAttributeUpdated( eAttr_IsPrepared ) )
    {
        asnMsg.m.en_preparationPresent = 1;
        asnMsg.en_preparation = bIsPrepared_;
    }     

    if( IsAttributeUpdated( eAttr_Dotations ) )
    {
        if( pObjectType_->GetDotationCategoryForConstruction() )
        {
            asnMsg.m.nb_dotation_constructionPresent = 1;
            asnMsg.nb_dotation_construction          = nNbrDotationForConstruction_;
        }
        if( pObjectType_->GetDotationCategoryForMining() )
        {
            asnMsg.m.nb_dotation_valorisationPresent = 1;
            asnMsg.nb_dotation_valorisation          = nNbrDotationForMining_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRealObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::BuildMsgRealObject( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const
{
    if( !IsAttributeUpdated( eAttr_RealObject ) )
        return;

    asnMsg.m.oid_objet_reelPresent = 1;
    asnMsg.oid_objet_reel = pObjectKnown_ ? pObjectKnown_->GetID() : 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateOnNetwork
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateOnNetwork()
{
    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
        NotifyAttributeUpdated( eAttr_CurrentPerceptionLevel );

    if( previousPerceptionPerAutomateSet_ != perceptionPerAutomateSet_ )
        NotifyAttributeUpdated( eAttr_PerceptionSources );

    if( nAttributesUpdated_ == eAttr_Nothing )
        return;

    NET_ASN_MsgObjectKnowledgeUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_connaissance    = nID_;
    
    assert( pArmyKnowing_ );
    asnMsg.GetAsnMsg().oid_camp_possesseur = pArmyKnowing_->GetID();
    
    BuildMsgRealObject            ( asnMsg.GetAsnMsg() );
    BuildMsgPerceptionSources     ( asnMsg.GetAsnMsg() );
    BuildMsgRelevance             ( asnMsg.GetAsnMsg() );
    BuildMsgLocalisations         ( asnMsg.GetAsnMsg() );
    BuildMsgCurrentPerceptionLevel( asnMsg.GetAsnMsg() );
    BuildMsgSpecificAttributes    ( asnMsg.GetAsnMsg() );
    BuildMsgStates                ( asnMsg.GetAsnMsg() );
    
    asnMsg.Send();

    if( asnMsg.GetAsnMsg().m.perception_par_compagniePresent && asnMsg.GetAsnMsg().perception_par_compagnie.n > 0 )
        delete [] asnMsg.GetAsnMsg().perception_par_compagnie.elem; //$$$ RAM

    if( asnMsg.GetAsnMsg().m.localisationPresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().localisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgCreation() const
{
    NET_ASN_MsgObjectKnowledgeCreation asnMsg;
    asnMsg.GetAsnMsg().oid_connaissance    = nID_;
    
    assert( pArmyKnowing_ );
    asnMsg.GetAsnMsg().oid_camp_possesseur = pArmyKnowing_->GetID();
    assert( pObjectType_ );
    asnMsg.GetAsnMsg().type                = pObjectType_->GetAsnID();

    if ( pObjectKnown_ )
        asnMsg.GetAsnMsg().oid_objet_reel = pObjectKnown_->GetID();
    else
        asnMsg.GetAsnMsg().oid_objet_reel = 0;

    if( pObjectType_->GetDotationCategoryForConstruction() )
    {
        asnMsg.GetAsnMsg().m.type_dotation_constructionPresent = 1;
        asnMsg.GetAsnMsg().type_dotation_construction          = pObjectType_->GetDotationCategoryForConstruction()->GetMosID();
    }

    if( pObjectType_->GetDotationCategoryForMining() )
    {
        asnMsg.GetAsnMsg().m.type_dotation_valorisationPresent = 1;
        asnMsg.GetAsnMsg().type_dotation_valorisation          = pObjectType_->GetDotationCategoryForMining()->GetMosID();
    }
    
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgDestruction() const
{
    NET_ASN_MsgObjectKnowledgeDestruction asnMsg;
    asnMsg.GetAsnMsg().oid_connaissance    = nID_;
    
    assert( pArmyKnowing_ );
    asnMsg.GetAsnMsg().oid_camp_possesseur = pArmyKnowing_->GetID();
    asnMsg.Send();
}
    

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendStateToNewClient()
{
    SendMsgCreation();
    NotifyAttributeUpdated( eAttr_AllAttributes );
    UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetMaxInteractionHeight
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Object::GetMaxInteractionHeight() const
{
    assert( pObjectType_ );
    return pObjectType_->GetMaxInteractionHeight();
}

// =============================================================================
// OEPRATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Recon( const MIL_Agent_ABC& agent )
{
    reconByAgentTypes_.insert( &agent.GetType() );
}
