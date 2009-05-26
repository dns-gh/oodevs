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
#include "DEC_Knowledge_ObjectAttribute_ABC.h"

#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/AvoidanceCapacity.h"
#include "Entities/Objects/ActivableCapacity.h"

#include "Entities/Automates/MIL_Automate.h"

#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"


#include "Entities/MIL_Army.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_IDManager.h"

#include "DEC_Knowledge_ObjectAttributeConstruction.h"
#include "DEC_Knowledge_ObjectAttributeBypass.h"
#include "DEC_Knowledge_ObjectAttributeObstacle.h"
#include "DEC_Knowledge_ObjectAttributeInteractionHeight.h"

#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_Object, "DEC_Knowledge_Object" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const MIL_Army_ABC& armyKnowing, MIL_Object_ABC& objectKnown )
    : DEC_Knowledge_ABC                 ()
    , pArmyKnowing_                     ( &armyKnowing )
    , pObjectKnown_                     ( &objectKnown )
    , pObjectType_                      ( &objectKnown.GetType() )
    , nID_                              ( MIL_IDManager::GetFreeId() )
    , nAttributesUpdated_               ( eAttr_AllAttributes )
    , pOwnerArmy_                       ( &objectKnown.GetArmy() )    
    , localisation_                     ( )
    , avoidanceLocalisation_            ( )
//    , bReservedObstacleActivated_       ( objectKnown.IsReservedObstacleActivated() ) //$$$ A CHIER ....              
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
    , nID_                             ( 0 )
    , nAttributesUpdated_              ( 0 )
    , pOwnerArmy_                      ( 0 )
    , localisation_                    ()
    , avoidanceLocalisation_           ()
    , pCurrentPerceptionLevel_         ( 0 )
    , pPreviousPerceptionLevel_        ( 0 )
    , pMaxPerceptionLevel_             ( 0 )
    , perceptionPerAutomateSet_        ()
    , previousPerceptionPerAutomateSet_()
    , nTimeLastUpdate_                 ( 0 )
    , rRelevance_                      ( 0. )
{
    // NOTHING
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
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this ); 
    
    std::string name;
    file >> name;
    pObjectType_ = &MIL_ObjectFactory::FindType( name );
    
    file >> const_cast< MIL_Army_ABC*& >( pArmyKnowing_ )
         >> pObjectKnown_
         >> const_cast< uint& >( nID_ )
         >> attributes_
         >> nAttributesUpdated_
         >> const_cast< MIL_Army_ABC*& >( pOwnerArmy_ )
         >> localisation_
         >> avoidanceLocalisation_;
         
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

    for ( IT_ObjectAttributeVector it = attributes_.begin(); it != attributes_.end(); ++it )
        (*it)->Register( *this );

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
    unsigned current      = pCurrentPerceptionLevel_->GetID(),
             previous     = pPreviousPerceptionLevel_->GetID(),
             max          = pMaxPerceptionLevel_->GetID();
    
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << pObjectType_->GetName();
    file << pArmyKnowing_;
    file << pObjectKnown_;
    file << nID_;
    file << attributes_;
    file << nAttributesUpdated_;
    file << pOwnerArmy_;
    file << localisation_;
    file << avoidanceLocalisation_;
    file << current;
    file << previous;
    file << max;
    file << perceptionPerAutomateSet_;
    file << previousPerceptionPerAutomateSet_;
    file << nTimeLastUpdate_;
    file << rRelevance_;
    
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
    if( !(localisation_ == localisation) )
    {
        localisation_.Reset( localisation );
        NotifyAttributeUpdated( eAttr_Localisation );
    }

    const AvoidanceCapacity* capacity = pObjectKnown_->Retrieve< AvoidanceCapacity >();
    if ( capacity )
    {
        const TER_Localisation& avoidanceLocalisation = capacity->GetLocalisation();
        if( !(avoidanceLocalisation_ == avoidanceLocalisation) )
        {
            avoidanceLocalisation_.Reset( avoidanceLocalisation );
        }
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

namespace 
{
    template< typename Container, typename Functor>
    void UpdateAttributes( Container& container, Functor functor )
    {
        std::for_each( container.begin(), container.end(), functor );
    }
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

    if( currentPerceptionLevel > PHY_PerceptionLevel::notSeen_ )
        UpdateAttributes( attributes_, boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::UpdateOnPerceptionLevel, _1, boost::ref( currentPerceptionLevel ) ) );
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
    UpdatePerceptionSources ( perception );

    if ( pObjectKnown_ && currentPerceptionLevel > PHY_PerceptionLevel::notSeen_ )
        UpdateAttributes( attributes_, boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::UpdateOnPerception, _1, boost::ref( perception ) ) );
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
    UpdateAttributes( attributes_, boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::UpdateOnCollision, _1, boost::ref( collision ) ) );
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
        attributes_.clear();
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
void DEC_Knowledge_Object::BuildMsgAttributes( ASN1T_MsgObjectKnowledgeUpdate& asn ) const
{
    assert( pObjectType_ );
    if( *pMaxPerceptionLevel_ == PHY_PerceptionLevel::notSeen_ )
        return;
    
    UpdateAttributes( attributes_, 
                      boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::Send, _1, boost::ref( asn.attributes ) ) );
    
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
    BuildMsgAttributes            ( asn() );
    
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
    
    asn().real_object = pObjectKnown_ ? pObjectKnown_->GetID() : 0;

    assert( pArmyKnowing_ );
    asn().team = pArmyKnowing_->GetID();
    asn().type = pObjectType_->GetName().c_str();

    std::for_each( attributes_.begin(), attributes_.end(), 
        boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::Send, _1, asn().attributes ) );

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
    const DEC_Knowledge_ObjectAttributeInteractionHeight* height = Retrieve< DEC_Knowledge_ObjectAttributeInteractionHeight >();
    if ( height )
        return height->Get();
    return 0;
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsBypassed
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsBypassed() const
{
    const DEC_Knowledge_ObjectAttributeBypass* bypass = Retrieve< DEC_Knowledge_ObjectAttributeBypass >();
    if ( bypass ) 
        return bypass->IsBypassed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsConstructed
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsConstructed() const
{
    const DEC_Knowledge_ObjectAttributeConstruction* construction = Retrieve< DEC_Knowledge_ObjectAttributeConstruction >();
    if ( construction ) 
        return construction->IsConstructed();
    return false;    
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsReservedObstacle
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsReservedObstacle() const
{
    return pObjectType_ && pObjectType_->GetCapacity< ActivableCapacity >();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsReservedObstacleActivated
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsReservedObstacleActivated() const
{
	const DEC_Knowledge_ObjectAttributeObstacle* activable = Retrieve< DEC_Knowledge_ObjectAttributeObstacle >();
	if ( activable ) 
		return activable->IsActivated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::CanCollideWith
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::CanCollideWith( const MIL_Agent_ABC& agent ) const
{
    return ( !IsReservedObstacle() || IsReservedObstacleActivated() ) && !IsBypassed() 
         && agent.GetRole< PHY_RolePion_Location >().GetHeight() <= GetMaxInteractionHeight();                    
}
