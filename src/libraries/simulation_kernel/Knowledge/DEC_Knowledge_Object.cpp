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
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/AvoidanceCapacity.h"
#include "Entities/Objects/ActivableCapacity.h"
#include "Entities/Objects/ContaminationCapacity.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "DEC_Knowledge_ObjectAttributeConstruction.h"
#include "DEC_Knowledge_ObjectAttributeBypass.h"
#include "DEC_Knowledge_ObjectAttributeObstacle.h"
#include "DEC_Knowledge_ObjectAttributeInteractionHeight.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Object )

MIL_IDManager DEC_Knowledge_Object::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const MIL_Army_ABC& armyKnowing, MIL_Object_ABC& objectKnown )
    : DEC_Knowledge_ABC()
    , pArmyKnowing_            ( &armyKnowing )
    , pGroupKnowing_           ( 0 )
    , pObjectKnown_            ( &objectKnown )
    , pObjectType_             ( &objectKnown.GetType() )
    , nID_                     ( idManager_.GetFreeId() )
    , nAttributesUpdated_      ( eAttr_AllAttributes )
    , pOwnerArmy_              ( objectKnown.GetArmy() )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_     ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_         ( 0 )
    , rRelevance_              ( 1. )
    , bValid_                  ( true )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const MIL_KnowledgeGroup& groupKnowing, MIL_Object_ABC& objectKnown )
    : DEC_Knowledge_ABC()
    , pArmyKnowing_            ( &groupKnowing.GetArmy() )
    , pGroupKnowing_           ( &groupKnowing )
    , pObjectKnown_            ( &objectKnown )
    , pObjectType_             ( &objectKnown.GetType() )
    , nID_                     ( idManager_.GetFreeId() )
    , nAttributesUpdated_      ( eAttr_AllAttributes )
    , pOwnerArmy_              ( objectKnown.GetArmy() )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_     ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_         ( 0 )
    , rRelevance_              ( 1. )
    , bValid_                  ( true )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object()
    : DEC_Knowledge_ABC()
    , pArmyKnowing_            ( 0 )
    , pGroupKnowing_           ( 0 )
    , pObjectKnown_            ( 0 )
    , pObjectType_             ( 0 )
    , nID_                     ( 0 )
    , nAttributesUpdated_      ( 0 )
    , pOwnerArmy_              ( 0 )
    , pCurrentPerceptionLevel_ ( 0 )
    , pPreviousPerceptionLevel_( 0 )
    , pMaxPerceptionLevel_     ( 0 )
    , nTimeLastUpdate_         ( 0 )
    , rRelevance_              ( 0. )
    , bValid_                  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const DEC_Knowledge_Object& copy, const MIL_KnowledgeGroup* pGroupKnowing )
    : DEC_Knowledge_ABC()
    , pArmyKnowing_                    ( copy.pArmyKnowing_ )
    , pGroupKnowing_                   ( pGroupKnowing )
    , pObjectKnown_                    ( copy.pObjectKnown_ )
    , pObjectType_                     ( copy.pObjectType_ )
    , nID_                             ( copy.idManager_.GetFreeId() )
    , nAttributesUpdated_              ( copy.nAttributesUpdated_ )
    , pOwnerArmy_                      ( copy.pOwnerArmy_ )
    , localisation_                    ( copy.localisation_ )
    , avoidanceLocalisation_           ( copy.avoidanceLocalisation_ )
    , pCurrentPerceptionLevel_         ( copy.pCurrentPerceptionLevel_ )
    , pPreviousPerceptionLevel_        ( copy.pPreviousPerceptionLevel_ )
    , pMaxPerceptionLevel_             ( copy.pMaxPerceptionLevel_ )
    , perceptionPerAutomateSet_        ( copy.perceptionPerAutomateSet_ )
    , previousPerceptionPerAutomateSet_( copy.previousPerceptionPerAutomateSet_ )
    , nTimeLastUpdate_                 ( copy.nTimeLastUpdate_ )
    , rRelevance_                      ( copy.rRelevance_ )
    , bValid_                          ( copy.bValid_ )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::~DEC_Knowledge_Object()
{
    Invalidate();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    std::string name;
    file >> name;
    pObjectType_ = &MIL_ObjectFactory::FindType( name );
    T_ObjectAttributeVector attributes;
    file >> const_cast< MIL_Army_ABC*& >( pArmyKnowing_ )
         >> pObjectKnown_
         >> const_cast< unsigned int& >( nID_ )
         >> attributes
         >> nAttributesUpdated_
         >> const_cast< MIL_Army_ABC*& >( pOwnerArmy_ )
         >> localisation_
         >> avoidanceLocalisation_;
    idManager_.Lock( nID_ );
    unsigned int nPerceptionID;
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
    for ( IT_ObjectAttributeVector it = attributes.begin(); it != attributes.end(); ++it )
        (*it)->Register( *this );

    // récupération des noms des types
    unsigned int nSize;
    file >> nSize;
    while ( nSize-- )
    {
        unsigned int nID;
        file >> nID;
        reconByAgentTypes_.insert( MIL_AgentTypePion::Find( nID ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pObjectType_ );
    unsigned current = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID(),
             max = pMaxPerceptionLevel_->GetID();
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
    unsigned int size = reconByAgentTypes_.size();
    file << size;
    for ( CIT_AgentTypeSet it = reconByAgentTypes_.begin(); it != reconByAgentTypes_.end(); ++it )
    {
        unsigned int id = (*it)->GetID();
        file << id;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Prepare()
{
    nAttributesUpdated_ = eAttr_Nothing;
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
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
    const TER_Localisation& localisation = pObjectKnown_->GetLocalisation();
    if( !(localisation_ == localisation) )
    {
        localisation_.Reset( localisation );
        NotifyAttributeUpdated( eAttr_Localisation );
    }
    const AvoidanceCapacity* capacity = pObjectKnown_->Retrieve< AvoidanceCapacity >();
    if( capacity )
    {
        const TER_Localisation& avoidanceLocalisation = capacity->GetLocalisation();
        if( !(avoidanceLocalisation_ == avoidanceLocalisation) )
            avoidanceLocalisation_.Reset( avoidanceLocalisation );
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
    const MIL_Agent_ABC& pionSource = perception.GetAgentPerceiving();
    MIL_Automate* pAutomateSource = const_cast< MIL_Automate* >( &perception.GetAgentPerceiving().GetAutomate() );
    // On ne garde que les sources provenant d'autres GTIAs
    IT_PerceptionSourceSet it = perceptionPerAutomateSet_.find( pAutomateSource );
    if( it == perceptionPerAutomateSet_.end() )
        perceptionPerAutomateSet_.insert( pAutomateSource );
    IT_PerceptionAgentSourceMap it2 = perceptionLevelPerAgentMap_.find( &pionSource );
    if( it2 == perceptionLevelPerAgentMap_.end() )
        perceptionLevelPerAgentMap_.insert( std::make_pair( &pionSource, &perception.GetCurrentPerceptionLevel() ) );
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
    UpdateMaxPerceptionLevel( currentPerceptionLevel );
    // $$$$ NLD 2007-02-07: currentPerceptionLevel peut valoir notSeen_ ?
    UpdateLocalisations();  // Updaté même quand 'NotPerceived', pour les objets pouvant bouger
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
    UpdateMaxPerceptionLevel( currentPerceptionLevel );
    // NB - Quand nPerceptionLevel vaut eNotPerceived => l'agent associé vient juste d'être perdu de vue
    //      => Pas de eNotPerceived aux ticks suivant la perte de contact
    UpdateLocalisations();// Updaté même quand 'NotPerceived', pour les objets pouvant bouger
    UpdatePerceptionSources( perception );
    if( pObjectKnown_ && currentPerceptionLevel > PHY_PerceptionLevel::notSeen_ )
        UpdateAttributes( attributes_, boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::UpdateOnPerception, _1, boost::ref( perception ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateLocalisationPartially
// Created: JCR 2009-12-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateLocalisationPartially( const DEC_Knowledge_ObjectCollision& collision )
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
    UpdateMaxPerceptionLevel( PHY_PerceptionLevel::identified_ );
    //$$$ TMP BULLSHIT
    if( !(localisation_ == pObjectKnown_->GetLocalisation() ) )
    {
        TER_Localisation::E_LocationType type = pObjectKnown_->GetLocalisation().GetType();
        if( type == TER_Localisation::ePoint || type == TER_Localisation::eLine )
            UpdateLocalisations();
        else
            UpdateLocalisationPartially( collision );
    }
    UpdateAttributes( attributes_, boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::UpdateOnCollision, _1, boost::ref( collision ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateRelevance()
{
    assert( rRelevance_ > 0. );
    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ && pObjectKnown_ && !pObjectKnown_->IsMarkedForDestruction() )
     {
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
    if( !pObjectKnown_ && pArmyKnowing_->IsPerceived( *this ) )
    {
        rRelevance_ = 0.;
        NotifyAttributeUpdated( eAttr_Relevance );
        return;
    }
    nTimeLastUpdate_ = GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgPerceptionSources
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgPerceptionSources( MsgsSimToClient::MsgObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_PerceptionSources ) )
        return;
    for( CIT_PerceptionSourceSet it = perceptionPerAutomateSet_.begin(); it != perceptionPerAutomateSet_.end(); ++it )
    {
        Common::AutomatId& data = *asn.mutable_perceiving_automats()->add_elem();
        data.set_id( (*it)->GetID() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRelevance
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgRelevance( MsgsSimToClient::MsgObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_Relevance ) )
        asn.set_relevance( (unsigned int)( rRelevance_ * 100. ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgLocalisations
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgLocalisations( MsgsSimToClient::MsgObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_Localisation ) )
        // $$$$ FDS 2010-04-16: Dans le cas où le type de localiation est none, la Localisation n'est pas renseigné ( Dans Gaming cela entrainerait sinon un Throw )
        if( localisation_.GetType() !=  Common::MsgLocation_Geometry_none )
            NET_ASN_Tools::WriteLocation( localisation_, *asn.mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel( MsgsSimToClient::MsgObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_CurrentPerceptionLevel ) )
        asn.set_perceived( ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgStates
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgAttributes( MsgsSimToClient::MsgObjectKnowledgeUpdate& asn ) const
{
    assert( pObjectType_ );
    asn.mutable_attributes();
    if( *pMaxPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
        UpdateAttributes( attributes_,
                      boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::Send, _1, boost::ref( *asn.mutable_attributes() ) ) );
    if( pObjectType_ && pObjectType_->GetCapacity< ContaminationCapacity >() && !IsRecon() && asn.attributes().has_nbc() )
        asn.mutable_attributes()->mutable_nbc()->set_danger_level( -1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRealObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgRealObject( MsgsSimToClient::MsgObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_RealObject ) )
        asn.mutable_object()->set_id( pObjectKnown_ ? pObjectKnown_->GetID() : 0 );
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

    client::ObjectKnowledgeUpdate asn;
    asn().mutable_id()->set_id( nID_ );
    assert( pArmyKnowing_ );
    asn().mutable_party()->set_id( pArmyKnowing_->GetID() );
    if( pGroupKnowing_ )
        asn().mutable_knowledge_group()->set_id( pGroupKnowing_->GetId() );

    BuildMsgRealObject( asn() );
    BuildMsgPerceptionSources( asn() );
    BuildMsgRelevance( asn() );
    BuildMsgLocalisations( asn() );
    BuildMsgCurrentPerceptionLevel( asn() );
    BuildMsgAttributes( asn() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgCreation() const
{
    client::ObjectKnowledgeCreation asn;
    asn().mutable_id()->set_id( nID_ );
    asn().mutable_object()->set_id( pObjectKnown_ ? pObjectKnown_->GetID() : 0 );
    assert( pArmyKnowing_ );
    asn().mutable_party()->set_id( pArmyKnowing_->GetID() );
    if( pGroupKnowing_ )
        asn().mutable_knowledge_group()->set_id( pGroupKnowing_->GetId() );
    asn().mutable_type()->set_id( pObjectType_->GetName().c_str() );
    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &DEC_Knowledge_ObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );

    if( pObjectType_ && pObjectType_->GetCapacity< ContaminationCapacity >() && !IsRecon() && asn().mutable_attributes()->has_nbc() )
        asn().mutable_attributes()->mutable_nbc()->set_danger_level( -1 );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgDestruction() const
{
    client::ObjectKnowledgeDestruction asn;
    asn().mutable_id()->set_id( nID_ );
    assert( pArmyKnowing_ );
    asn().mutable_party()->set_id( pArmyKnowing_->GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
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
    if( height )
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
    NotifyAttributeUpdated( eAttr_Specific );
    UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsBypassed
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsBypassed() const
{
    const DEC_Knowledge_ObjectAttributeBypass* bypass = Retrieve< DEC_Knowledge_ObjectAttributeBypass >();
    if( bypass )
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
    if( construction )
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
    if( activable )
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
         && agent.GetRole< PHY_RoleInterface_Location >().GetHeight() <= GetMaxInteractionHeight();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_Object::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetArmy
// Created: NLD 2006-01-05
// -----------------------------------------------------------------------------
const MIL_Army_ABC& DEC_Knowledge_Object::GetArmy() const
{
    assert( pOwnerArmy_ );
    return *pOwnerArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetLocalisation
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_Knowledge_Object::GetLocalisation() const
{
    return localisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetAvoidanceLocalisation
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_Knowledge_Object::GetAvoidanceLocalisation() const
{
    return avoidanceLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetType
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& DEC_Knowledge_Object::GetType() const
{
    assert( pObjectType_ );
    return *pObjectType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Object::GetRelevance() const
{
    return rRelevance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetObjectKnown
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
MIL_Object_ABC* DEC_Knowledge_Object::GetObjectKnown() const
{
    return pObjectKnown_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::NotifyAttributeUpdated
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::NotifyAttributeUpdated( E_Attributes nAttribute )
{
    nAttributesUpdated_ |= nAttribute;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsAttributeUpdated
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsAttributeUpdated( E_Attributes nAttribute ) const
{
    return ( nAttributesUpdated_ & nAttribute ) == nAttribute;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsReconBy
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsReconBy( const MIL_AgentType_ABC& agentType ) const
{
    return reconByAgentTypes_.find( &agentType ) != reconByAgentTypes_.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsRecon() const
{
    return !reconByAgentTypes_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::Clean() const
{
    return GetRelevance() <= 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetArmyKnowing
// Created: NLD 2007-02-07
// -----------------------------------------------------------------------------
const MIL_Army_ABC& DEC_Knowledge_Object::GetArmyKnowing() const
{
    assert( pArmyKnowing_ );
    return *pArmyKnowing_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsValid
// Created: LDC 2009-10-22
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsValid() const
{
    return bValid_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Invalidate
// Created: LDC 2009-10-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Invalidate()
{
    if( bValid_ )
    {
        bValid_ = false;
        if( pObjectType_ )
            SendMsgDestruction();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsAnEnemy
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
E_Tristate DEC_Knowledge_Object::IsAnEnemy( const MIL_Army_ABC& army ) const
{
    return army.IsAnEnemy( GetArmy() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsAFriend
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
E_Tristate DEC_Knowledge_Object::IsAFriend( const MIL_Army_ABC& army ) const
{
    return army.IsAFriend( GetArmy() );
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Object::GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const
{
    CIT_PerceptionAgentSourceMap  itPerceptionLevel = perceptionLevelPerAgentMap_.find( &pion );
    if( itPerceptionLevel != perceptionLevelPerAgentMap_.end() )
        return *( itPerceptionLevel->second );
    return PHY_PerceptionLevel::notSeen_;
}