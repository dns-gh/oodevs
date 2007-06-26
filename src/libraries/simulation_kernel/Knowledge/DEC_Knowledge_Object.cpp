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
    , pObstacleType_                    ( objectKnown.GetObstacleType() )
    , nID_                              ( pObjectType_->GetIDManager().GetFreeSimID() )
    , nAttributesUpdated_               ( eAttr_AllAttributes )
    , pOwnerArmy_                       ( &objectKnown.GetArmy() )    
    , localisation_                     ( )
    , avoidanceLocalisation_            ( )
    , nConstructionPercentage_          ( 0 )
    , nMiningPercentage_                ( 0 )
    , nBypassPercentage_                ( 0 )
    , bReservedObstacleActivated_       ( objectKnown.IsReservedObstacleActivated() ) //$$$ A CHIER ....
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
    , pArmyKnowing_                    ( 0 )
    , pObjectKnown_                    ( 0 )
    , pObjectType_                     ( 0 )
    , pObstacleType_                   ( 0 )
    , nID_                             ( 0 )
    , nAttributesUpdated_              ( 0 )
    , pOwnerArmy_                      ( 0 )
    , localisation_                    ()
    , avoidanceLocalisation_           ()
    , nConstructionPercentage_         ( 0 )
    , nMiningPercentage_               ( 0 )
    , nBypassPercentage_               ( 0 )
    , bReservedObstacleActivated_      ( false )
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
    pObjectType_ = MIL_RealObjectType::Find( nObjectTypeID );
    assert( pObjectType_ );

    uint nObstacleTypeID;
    file >> nObstacleTypeID;
    pObstacleType_ = 0;
    if( nObstacleTypeID != (uint)-1 )
    {
        pObstacleType_ = MIL_ObstacleType::Find( nObstacleTypeID );
        assert( pObstacleType_ );
    }

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
         >> bReservedObstacleActivated_
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
        reconByAgentTypes_.insert( MIL_AgentTypePion::Find( nID ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pObjectType_ );
    unsigned objectType   = pObjectType_->GetID(),
             obstacleType = ( pObstacleType_ ? pObstacleType_->GetID() : (uint)-1 ),
             current      = pCurrentPerceptionLevel_->GetID(),
             previous     = pPreviousPerceptionLevel_->GetID(),
             max          = pMaxPerceptionLevel_->GetID();
    
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << objectType
         << obstacleType
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
         << bReservedObstacleActivated_
         << nNbrDotationForConstruction_
         << nNbrDotationForMining_
         << current
         << previous
         << max
         << perceptionPerAutomateSet_
         << previousPerceptionPerAutomateSet_
         << nTimeLastUpdate_
         << rRelevance_;
    
    // On stocke les types selon leur nom
    uint size = reconByAgentTypes_.size();
    file << size;
    for ( CIT_AgentTypeSet it = reconByAgentTypes_.begin(); it != reconByAgentTypes_.end(); ++it )
    {
        uint id = (*it)->GetID();
        file << id;
    }
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

    if( pObjectKnown_->IsReservedObstacleActivated() != bReservedObstacleActivated_ )
    {
        bReservedObstacleActivated_ = pObjectKnown_->IsReservedObstacleActivated();
        NotifyAttributeUpdated( eAttr_ReservedObstacleActivated );
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
    assert( currentPerceptionLevel > PHY_PerceptionLevel::notSeen_ );

    nTimeLastUpdate_ = GetCurrentTimeStep();
    
    UpdateCurrentPerceptionLevel( currentPerceptionLevel );
    UpdateMaxPerceptionLevel    ( currentPerceptionLevel );

    // $$$$ NLD 2007-02-07: currentPerceptionLevel peut valoir notSeen_ ?
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
    UpdateMaxPerceptionLevel    ( currentPerceptionLevel );

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
 
    //$$$ TMP BULLSHIT
    if( !(localisation_ == pObjectKnown_->GetLocalisation() ) )
    {
        if( std::find( localisation_.GetPoints().begin(), localisation_.GetPoints().end(), collision.GetPosition() ) == localisation_.GetPoints().end() )
        {
            T_PointVector points = localisation_.GetPoints();
            while( points.size() > 10 )  // $$$$ NLD 2007-05-07: 10 : why not ...
                points.erase( points.begin() );
            points.push_back( collision.GetPosition() );
            if( points.size() > 2 )
                points.push_back( points.front() );
            localisation_.Reset( points );
            localisation_.Convexify();

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
void DEC_Knowledge_Object::BuildMsgPerceptionSources( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_PerceptionSources ) )
        return;

    asn.m.automat_perceptionPresent = 1;
    asn.automat_perception.n        = perceptionPerAutomateSet_.size();

    if( perceptionPerAutomateSet_.empty() )
        return;

    ASN1T_OID* pPerceptions = new ASN1T_OID[ perceptionPerAutomateSet_.size() ]; //$$ RAM
    uint i = 0;
    for( CIT_PerceptionSourceSet it = perceptionPerAutomateSet_.begin(); it != perceptionPerAutomateSet_.end(); ++it )
    {
        pPerceptions[i] = (*it)->GetID();
        ++i;
    }
    asn.automat_perception.elem = pPerceptions;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRelevance
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgRelevance( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_Relevance ) )
        return;

    asn.m.relevancePresent = 1;
    asn.relevance = (uint)( rRelevance_ * 100. );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgLocalisations
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgLocalisations( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_Localisation ) )
        return;

    asn.m.locationPresent = 1;
    NET_ASN_Tools::WriteLocation( localisation_, asn.location );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_CurrentPerceptionLevel ) )
        return;

    asn.m.perceivedPresent = 1;
    asn.perceived = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgStates
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgStates( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    assert( pObjectType_ );
    if( *pMaxPerceptionLevel_ == PHY_PerceptionLevel::notSeen_ )
        return;

    if( IsAttributeUpdated( eAttr_ConstructionPercentage ) )
    {
        asn.m.construction_percentagePresent = 1;
        asn.construction_percentage = nConstructionPercentage_;
    }

    if( IsAttributeUpdated( eAttr_MiningPercentage ) )
    {
        asn.m.mining_percentagePresent = 1;
        asn.mining_percentage = nMiningPercentage_;
    }

    if( IsAttributeUpdated( eAttr_BypassPercentage ) )
    {
        asn.m.bypass_construction_percentagePresent = 1;
        asn.bypass_construction_percentage = nBypassPercentage_;
    }

    if( IsAttributeUpdated( eAttr_ReservedObstacleActivated ) && IsReservedObstacle() )
    {
        asn.m.reserved_obstacle_activatedPresent = 1;
        asn.reserved_obstacle_activated          = bReservedObstacleActivated_;
    }     

    if( IsAttributeUpdated( eAttr_Dotations ) )
    {
        if( pObjectType_->GetDotationCategoryForConstruction() )
        {
            asn.m.construction_dotation_nbrPresent = 1;
            asn.construction_dotation_nbr          = nNbrDotationForConstruction_;
        }
        if( pObjectType_->GetDotationCategoryForMining() )
        {
            asn.m.mining_dotation_nbrPresent = 1;
            asn.mining_dotation_nbr          = nNbrDotationForMining_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRealObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::BuildMsgRealObject( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_RealObject ) )
        return;

    asn.m.real_objectPresent = 1;
    asn.real_object = pObjectKnown_ ? pObjectKnown_->GetID() : 0;
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

    NET_ASN_MsgObjectKnowledgeUpdate asn;
    asn().oid    = nID_;
    
    assert( pArmyKnowing_ );
    asn().team = pArmyKnowing_->GetID();
    
    BuildMsgRealObject            ( asn() );
    BuildMsgPerceptionSources     ( asn() );
    BuildMsgRelevance             ( asn() );
    BuildMsgLocalisations         ( asn() );
    BuildMsgCurrentPerceptionLevel( asn() );
    BuildMsgSpecificAttributes    ( asn() );
    BuildMsgStates                ( asn() );
    
    asn.Send();

    if( asn().m.automat_perceptionPresent && asn().automat_perception.n > 0 )
        delete [] asn().automat_perception.elem; //$$$ RAM

    if( asn().m.locationPresent )
        NET_ASN_Tools::Delete( asn().location );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgCreation() const
{
    NET_ASN_MsgObjectKnowledgeCreation asn;
    asn().oid    = nID_;
    
    assert( pArmyKnowing_ );
    asn().team = pArmyKnowing_->GetID();
    assert( pObjectType_ );
    asn().type                = pObjectType_->GetAsnID();

    if( pObstacleType_ )
    {
        asn().m.obstacle_typePresent = 1;
        asn().obstacle_type          = pObstacleType_->GetAsnID();
        if( pObstacleType_->CouldBeActivated() )
        {
            asn().m.reserved_obstacle_activatedPresent = 1;
            asn().reserved_obstacle_activated          = bReservedObstacleActivated_;
        }
    }

    if( pObjectKnown_ )
        asn().real_object = pObjectKnown_->GetID();
    else
        asn().real_object = 0;

    if( pObjectType_->GetDotationCategoryForConstruction() )
    {
        asn().m.construction_dotation_typePresent = 1;
        asn().construction_dotation_type          = pObjectType_->GetDotationCategoryForConstruction()->GetMosID();
    }

    if( pObjectType_->GetDotationCategoryForMining() )
    {
        asn().m.mining_dotation_typePresent = 1;
        asn().mining_dotation_type          = pObjectType_->GetDotationCategoryForMining()->GetMosID();
    }
    
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgDestruction() const
{
    NET_ASN_MsgObjectKnowledgeDestruction asn;
    asn().oid    = nID_;
    
    assert( pArmyKnowing_ );
    asn().team = pArmyKnowing_->GetID();
    asn.Send();
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
