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
#include "DEC_Knowledge_ObjectMagicPerception.h"
#include "MIL_Time_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/ActivableCapacity.h"
#include "Entities/Objects/AvoidanceCapacity.h"
#include "Entities/Objects/BypassAttribute.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/CrossingSiteAttribute.h"
#include "Entities/Objects/CrowdCapacity.h"
#include "Entities/Objects/FloodAttribute.h"
#include "Entities/Objects/InteractionHeightAttribute.h"
#include "Entities/Objects/InteractWithSideCapacity.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/MobilityCapacity.h"
#include "Entities/Objects/NBCAttribute.h"
#include "Entities/Objects/ObstacleAttribute.h"
#include "Entities/Objects/TrafficabilityAttribute.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/optional.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Object )

MIL_IDManager DEC_Knowledge_Object::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const MIL_Army_ABC& armyKnowing, MIL_Object_ABC& objectKnown, bool sendCreation /*= true*/ )
    : pArmyKnowing_            ( &armyKnowing )
    , pObjectKnown_            ( &objectKnown )
    , objectId_                ( objectKnown.GetID() )
    , pObjectType_             ( &objectKnown.GetType() )
    , nID_                     ( idManager_.GetId() )
    , name_                    ( objectKnown.GetName() )
    , nAttributesUpdated_      ( eAttr_AllAttributes )
    , pOwnerArmy_              ( objectKnown.GetArmy() )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_     ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_         ( 0 )
    , rRelevance_              ( 1. )
    , bValid_                  ( true )
    , bPerceptionDistanceHacked_ ( false )
    , bSkipPreparation_        ( false )
    , locationType_            ( pObjectKnown_->GetLocalisation().GetType() )
{
    if( sendCreation )
        SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const MIL_KnowledgeGroup& group, MIL_Object_ABC& objectKnown, double rRelevance )
    : pArmyKnowing_            ( &group.GetArmy() )
    , pObjectKnown_            ( &objectKnown )
    , objectId_                ( objectKnown.GetID() )
    , pObjectType_             ( &objectKnown.GetType() )
    , nID_                     ( idManager_.GetId() )
    , groupId_                 ( group.GetId() )
    , name_                    ( objectKnown.GetName() )
    , nAttributesUpdated_      ( eAttr_AllAttributes )
    , pOwnerArmy_              ( objectKnown.GetArmy() )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_     ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_         ( 0 )
    , rRelevance_              ( rRelevance )
    , bValid_                  ( true )
    , bPerceptionDistanceHacked_ ( false )
    , bSkipPreparation_        ( false )
    , locationType_            ( pObjectKnown_->GetLocalisation().GetType() )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object()
    : pArmyKnowing_            ( 0 )
    , pObjectKnown_            ( 0 )
    , objectId_                ( 0 )
    , pObjectType_             ( 0 )
    , nID_                     ( 0 )
    , name_                    ( "unknown" )
    , nAttributesUpdated_      ( 0 )
    , pOwnerArmy_              ( 0 )
    , pCurrentPerceptionLevel_ ( 0 )
    , pPreviousPerceptionLevel_( 0 )
    , pMaxPerceptionLevel_     ( 0 )
    , nTimeLastUpdate_         ( 0 )
    , rRelevance_              ( 0. )
    , bValid_                  ( true )
    , bPerceptionDistanceHacked_ ( false )
    , bSkipPreparation_        ( false )
    , locationType_            ( TER_Localisation::eNone )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const DEC_Knowledge_Object& copy, const MIL_KnowledgeGroup& group )
    : DEC_Knowledge_ABC()
    , pArmyKnowing_                    ( copy.pArmyKnowing_ )
    , pObjectKnown_                    ( copy.pObjectKnown_ )
    , objectId_                        ( copy.objectId_ )
    , pObjectType_                     ( copy.pObjectType_ )
    , nID_                             ( copy.idManager_.GetId() )
    , groupId_                         ( group.GetId() )
    , name_                            ( copy.name_ )
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
    , bPerceptionDistanceHacked_       ( copy.bPerceptionDistanceHacked_ )
    , bSkipPreparation_                ( false )
    , locationType_                    ( copy.locationType_ )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object constructor
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_Object::DEC_Knowledge_Object( const DEC_Knowledge_Object& copy, const MIL_Army_ABC& armyKnowing )
    : DEC_Knowledge_ABC()
    , pArmyKnowing_                    ( &armyKnowing )
    , pObjectKnown_                    ( copy.pObjectKnown_ )
    , objectId_                        ( copy.objectId_ )
    , pObjectType_                     ( copy.pObjectType_ )
    , nID_                             ( copy.idManager_.GetId() )
    , name_                            ( copy.name_ )
    , nAttributesUpdated_              ( eAttr_AllAttributes )
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
    , bPerceptionDistanceHacked_       ( copy.bPerceptionDistanceHacked_ )
    , bSkipPreparation_                ( false )
    , locationType_                    ( copy.locationType_ )
{
    SendMsgCreation();
    SendUpdateOnNetwork();
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
    pObjectType_ = &file.GetObjectTypeResolver().FindType( name );
    file >> const_cast< MIL_Army_ABC*& >( pArmyKnowing_ );
    file >> pObjectKnown_;
    file >> const_cast< unsigned int& >( objectId_ );
    file >> const_cast< unsigned int& >( nID_ );
    file >> groupId_;
    file >> name_;
    file >> nAttributesUpdated_;
    file >> const_cast< MIL_Army_ABC*& >( pOwnerArmy_ );
    file >> localisation_;
    file >> avoidanceLocalisation_;
    idManager_.GetId( nID_, true );
    file >> pCurrentPerceptionLevel_;
    file >> pPreviousPerceptionLevel_;
    file >> pMaxPerceptionLevel_;
    file >> perceptionPerAutomateSet_;
    file >> previousPerceptionPerAutomateSet_;
    file >> nTimeLastUpdate_;
    file >> rRelevance_;
    file >> bValid_;
    file >> bPerceptionDistanceHacked_;
    file >> bSkipPreparation_;
    file >> locationType_;

    // récupération des noms des types
    std::size_t nSize;
    file >> nSize;
    while( nSize-- )
    {
        unsigned int nID;
        file >> nID;
        reconByAgentTypes_.insert( MIL_AgentTypePion::Find( nID ) );
    }

    // Attributes
    std::vector< DEC_Knowledge_IObjectAttributeProxy* > attributes;
    file >> attributes;
    for( std::vector< DEC_Knowledge_IObjectAttributeProxy* >::const_iterator it = attributes.begin(); it != attributes.end(); ++it )
        ( *it )->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pObjectType_ );
    std::string name = pObjectType_->GetName();
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << name;
    file << pArmyKnowing_;
    file << pObjectKnown_;
    file << objectId_;
    file << nID_;
    file << groupId_;
    file << name_;
    file << nAttributesUpdated_;
    file << pOwnerArmy_;
    file << localisation_;
    file << avoidanceLocalisation_;
    file << pCurrentPerceptionLevel_;
    file << pPreviousPerceptionLevel_;
    file << pMaxPerceptionLevel_;
    file << perceptionPerAutomateSet_;
    file << previousPerceptionPerAutomateSet_;
    file << nTimeLastUpdate_;
    file << rRelevance_;
    file << bValid_;
    file << bPerceptionDistanceHacked_;
    file << bSkipPreparation_;
    file << locationType_;

    // On stocke les types selon leur nom
    std::size_t size = reconByAgentTypes_.size();
    for( auto it = reconByAgentTypes_.begin(); it != reconByAgentTypes_.end(); ++it )
    {
        if( !*it )
            --size;
    }
    file << size;
    for( auto it = reconByAgentTypes_.begin(); it != reconByAgentTypes_.end(); ++it )
    {
        if( !*it )
            continue;
        unsigned int id = ( *it )->GetID();
        file << id;
    }

    // Attributes
    std::vector< DEC_Knowledge_IObjectAttributeProxy* > attributes;
    for( auto it = extensions_.Container().begin(); it != extensions_.Container().end(); ++it )
        if( *it )
            attributes.push_back( *it );
    file << attributes;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::WriteKnowledge
// Created: NPT 2012-08-08
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::WriteKnowledges( xml::xostream& xos ) const
{
    if( !bValid_ )
        return;

    xos << xml::start( "object-knowledge" )
        << xml::attribute( "id", nID_ )
        << xml::attribute( "name", name_ )
        << xml::attribute( "object-id", objectId_ );
    if( pObjectType_ )
        xos << xml::attribute( "type", pObjectType_->GetID() );
    if( pCurrentPerceptionLevel_ )
        xos << xml::attribute( "level", pCurrentPerceptionLevel_->GetID() );
    if( pMaxPerceptionLevel_ )
        xos << xml::attribute( "max-level", pMaxPerceptionLevel_->GetID() );
    xos  << xml::attribute( "last-time-update", nTimeLastUpdate_ )
         << xml::attribute( "relevance", rRelevance_ );
    if( bPerceptionDistanceHacked_ )
        xos << xml::attribute( "hacked", bPerceptionDistanceHacked_ );
    if( bSkipPreparation_ )
        xos << xml::attribute( "skip-preparation", bSkipPreparation_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Prepare()
{
    if( bSkipPreparation_ )
    {
        bSkipPreparation_ = false;
        return;
    }
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
void DEC_Knowledge_Object::UpdateLocalisations()
{
    if( pObjectKnown_ )
        UpdateLocalisations( *pObjectKnown_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateLocalisations
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateLocalisations( const MIL_Object_ABC& pObjectKnown )
{
    const TER_Localisation& localisation = const_cast< const MIL_Object_ABC& >( pObjectKnown ).GetLocalisation();
    if( !( localisation_ == localisation ) )
    {
        localisation_.Reset( localisation );
        NotifyAttributeUpdated( eAttr_Localisation );
    }
    const AvoidanceCapacity* capacity = pObjectKnown.Retrieve< AvoidanceCapacity >();
    if( capacity )
    {
        const TER_Localisation& avoidanceLocalisation = capacity->GetLocalisation();
        if( !( avoidanceLocalisation_ == avoidanceLocalisation ) )
            avoidanceLocalisation_.Reset( avoidanceLocalisation );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdatePerceptionSources
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdatePerceptionSources( const DEC_Knowledge_ObjectPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;
    const MIL_Agent_ABC& pionSource = perception.GetAgentPerceiving();
    MIL_Automate* pAutomateSource = const_cast< MIL_Automate* >( &perception.GetAgentPerceiving().GetAutomate() );
    // On ne garde que les sources provenant d'autres GTIAs
    perceptionPerAutomateSet_.insert( pAutomateSource );
    perceptionLevelPerAgentMap_.insert( std::make_pair( &pionSource, &perception.GetCurrentPerceptionLevel() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateCurrentPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateMaxPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
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
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    UpdateCurrentPerceptionLevel( currentPerceptionLevel );
    UpdateMaxPerceptionLevel( currentPerceptionLevel );
    // $$$$ NLD 2007-02-07: currentPerceptionLevel peut valoir notSeen_ ?
    UpdateLocalisations();  // Updaté même quand 'NotPerceived', pour les objets pouvant bouger
    DEC_Knowledge_ObjectMagicPerception param( *this, currentPerceptionLevel );
    UpdateAttributes( boost::bind( &DEC_Knowledge_IObjectAttributeProxy::UpdateOnPerceptionLevel, _1, boost::ref( *this ), boost::ref( param ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::EmitDetectionReport
// Created: LDC 2014-09-24
// -----------------------------------------------------------------------------
template< typename T > void DEC_Knowledge_Object::EmitDetectionReport( const T& emitter ) const
{
    boost::shared_ptr< DEC_Knowledge_Object > shared_this;
    if( groupId_ )
    {
        auto knowledgeGroup = pArmyKnowing_->FindKnowledgeGroup( *groupId_ );
        if( knowledgeGroup )
            shared_this = knowledgeGroup->ResolveKnowledgeObjectByObjectID( objectId_ );
    }
    else if( pArmyKnowing_ )
        shared_this = pArmyKnowing_->GetKnowledge().ResolveKnowledgeObjectByObjectID( objectId_ );
    if( !shared_this )
    {
        MT_LOG_ERROR_MSG( "Object knowledge with neither an army nor a knowledge group!" );
    }
    else
        MIL_Report::PostEvent( emitter, report::eRC_DetectedObject, shared_this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Update
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Update( const DEC_Knowledge_ObjectPerception& perception )
{
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    const PHY_PerceptionLevel& currentPerceptionLevel = perception.GetCurrentPerceptionLevel();
    UpdateCurrentPerceptionLevel( currentPerceptionLevel );
    if( UpdateMaxPerceptionLevel( currentPerceptionLevel ) )
        EmitDetectionReport( perception.GetAgentPerceiving() );
    // NB - Quand nPerceptionLevel vaut eNotPerceived => l'agent associé vient juste d'être perdu de vue
    //      => Pas de eNotPerceived aux ticks suivant la perte de contact
    UpdateLocalisations();// Updaté même quand 'NotPerceived', pour les objets pouvant bouger
    UpdatePerceptionSources( perception );
    UpdateAttributes( boost::bind( &DEC_Knowledge_IObjectAttributeProxy::UpdateOnPerception, _1, boost::ref( *this ), boost::ref( perception ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateFromCrowdPerception
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateFromCrowdPerception( const MIL_Population& crowd )
{
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    UpdateCurrentPerceptionLevel( PHY_PerceptionLevel::recognized_ );
    if( UpdateMaxPerceptionLevel( PHY_PerceptionLevel::recognized_ ) )
        EmitDetectionReport( crowd );
    UpdateLocalisations();// Updaté même quand 'NotPerceived', pour les objets pouvant bouger    
    if( pObjectKnown_ && !pObjectKnown_->IsMarkedForDestruction() )
    {
        DEC_Knowledge_ObjectCollision collision( *pObjectKnown_ );
        UpdateAttributes( boost::bind( &DEC_Knowledge_IObjectAttributeProxy::UpdateOnCollision, _1, boost::ref( *this ), boost::ref( collision ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateUniversalKnowledge
// Created: JSR 2013-12-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateUniversalKnowledge()
{
    if( pObjectKnown_ && pObjectKnown_->IsUniversal() )
        Update( PHY_PerceptionLevel::identified_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::ForceUpdate
// Created: JSR 2014-08-08
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::ForceUpdate()
{
    DEC_Knowledge_ObjectMagicPerception param( *this, PHY_PerceptionLevel::identified_ );
    UpdateAttributes( boost::bind( &DEC_Knowledge_IObjectAttributeProxy::UpdateOnPerceptionLevel, _1, boost::ref( *this ), boost::ref( param ) ) );
    UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateLocalisationPartially
// Created: JCR 2009-12-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::UpdateLocalisationPartially( const DEC_Knowledge_ObjectCollision& collision, const TER_Localisation& realLocation )
{
    if( std::find( localisation_.GetPoints().begin(), localisation_.GetPoints().end(), collision.GetPosition() ) == localisation_.GetPoints().end() )
    {
        T_PointVector points = localisation_.GetPoints();
        MT_Vector2D collisionPoint = collision.GetPosition();
        T_PointVector scaledVector;
        MT_Vector2D deltaX( 10, 0 );
        MT_Vector2D deltaY( 0, 10 );
        scaledVector.push_back( collisionPoint + deltaX + deltaY );
        scaledVector.push_back( collisionPoint + deltaX - deltaY );
        scaledVector.push_back( collisionPoint - deltaX - deltaY );
        scaledVector.push_back( collisionPoint - deltaX + deltaY );
        scaledVector.push_back( collisionPoint + deltaX + deltaY );
        TER_Polygon polygon;
        polygon.Reset( scaledVector );
        TER_Localisation location( polygon );
        location.GetPointsClippedByLocalisation( realLocation, scaledVector );
        for( auto it = scaledVector.begin(); it != scaledVector.end(); ++it )
            points.push_back( *it );
        if( points.size() >= 2 )
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
    if( !collision.IsValid() || !pObjectKnown_ )
        return;
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( UpdateMaxPerceptionLevel( PHY_PerceptionLevel::identified_ ) )
    {
        auto agent = collision.GetAgentColliding();
        if( agent )
            EmitDetectionReport( *agent );
    }
    //$$$ TMP BULLSHIT
    if( !( localisation_ == pObjectKnown_->GetLocalisation() ) )
    {
        TER_Localisation::E_LocationType type = pObjectKnown_->GetLocalisation().GetType();
        if( type == TER_Localisation::ePoint || type == TER_Localisation::eLine )
            UpdateLocalisations();
        else
            UpdateLocalisationPartially( collision, pObjectKnown_->GetLocalisation() );
    }
    UpdateAttributes( boost::bind( &DEC_Knowledge_IObjectAttributeProxy::UpdateOnCollision, _1, boost::ref( *this ), boost::ref( collision ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Update
// Created: MMC 2013-070-3
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Update( const DEC_Knowledge_Object& knowledge, int currentTimeStep )
{
    nTimeLastUpdate_ = currentTimeStep;

    for( auto it = knowledge.perceptionPerAutomateSet_.begin(); it != knowledge.perceptionPerAutomateSet_.end(); ++it )
        perceptionPerAutomateSet_.insert( *it );
    if( previousPerceptionPerAutomateSet_ != perceptionPerAutomateSet_ )
        NotifyAttributeUpdated( eAttr_PerceptionSources );

    if( knowledge.pObjectKnown_ )
        UpdateLocalisations( *knowledge.pObjectKnown_ );

    double newRelevance = std::max( rRelevance_, knowledge.GetRelevance() );
    if( newRelevance != rRelevance_ )
    {
        rRelevance_ = newRelevance;
        NotifyAttributeUpdated( eAttr_Relevance );
    }
    const PHY_PerceptionLevel* newCurLevel = &std::max( *pCurrentPerceptionLevel_, knowledge.GetCurrentPerceptionLevel() );
    if( newCurLevel != pMaxPerceptionLevel_ )
    {
        pCurrentPerceptionLevel_ = newCurLevel;
        NotifyAttributeUpdated( eAttr_CurrentPerceptionLevel );
    }
    const PHY_PerceptionLevel* newMaxLevel = &std::max( *pMaxPerceptionLevel_, *knowledge.pMaxPerceptionLevel_ );
    if( newMaxLevel != pMaxPerceptionLevel_ )
    {
        pMaxPerceptionLevel_ = newMaxLevel;
        NotifyAttributeUpdated( eAttr_MaxPerceptionLevel );
    }
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
    CleanObjectKnown();

    // Si plus d'objet réel associé est si l'emplacement de l'objet est vu
    assert( pArmyKnowing_ );
    if( !pObjectKnown_ && pArmyKnowing_ && pArmyKnowing_->IsPerceived( *this ) )
    {
        rRelevance_ = 0.;
        NotifyAttributeUpdated( eAttr_Relevance );
        return;
    }
    if( bPerceptionDistanceHacked_ )
        UpdateLocalisations();
    nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgPerceptionSources
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgPerceptionSources( sword::ObjectKnowledgeUpdate& asn ) const
{
    if( !IsAttributeUpdated( eAttr_PerceptionSources ) )
        return;
    for( auto it = perceptionPerAutomateSet_.begin(); it != perceptionPerAutomateSet_.end(); ++it )
        asn.mutable_perceiving_automats()->add_elem()->set_id( ( *it )->GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRelevance
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgRelevance( sword::ObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_Relevance ) )
        asn.set_relevance( static_cast< unsigned int >( rRelevance_ * 100. ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgLocalisations
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgLocalisations( sword::ObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_Localisation ) )
        // $$$$ FDS 2010-04-16: Dans le cas où le type de localiation est none, la Localisation n'est pas renseigné ( Dans Gaming cela entrainerait sinon un Throw )
        if( localisation_.GetType() != sword::Location_Geometry_none )
            NET_ASN_Tools::WriteLocation( localisation_, *asn.mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgCurrentPerceptionLevel( sword::ObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_CurrentPerceptionLevel ) )
        asn.set_perceived( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgStates
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgAttributes( sword::ObjectKnowledgeUpdate& asn ) const
{
    // $$$$ NLD 2010-10-26: Bullshit, mais faut ce qu'il faut pour réparer le mer(d)ge des objets
    if( nAttributesUpdated_ == eAttr_AllAttributes )
    {
        for( std::vector< DEC_Knowledge_IObjectAttributeProxy* >::const_iterator it = extensions_.Container().begin(); it != extensions_.Container().end(); ++it )
            if( *it )
                ( *it )->SendFullState( *asn.mutable_attributes() );
    }
    else if( IsAttributeUpdated( eAttr_Attributes ) )
        for( std::vector< DEC_Knowledge_IObjectAttributeProxy* >::const_iterator it = extensions_.Container().begin(); it != extensions_.Container().end(); ++it )
            if( *it )
                ( *it )->SendChangedState( *asn.mutable_attributes() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgRealObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::BuildMsgRealObject( sword::ObjectKnowledgeUpdate& asn ) const
{
    if( IsAttributeUpdated( eAttr_RealObject ) )
        asn.mutable_object()->set_id( objectId_ );
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
    SendUpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateOnNetwork
// Created: NPT 2012-12-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendUpdateOnNetwork()
{
    client::ObjectKnowledgeUpdate asn;
    asn().mutable_knowledge()->set_id( nID_ );
    asn().mutable_party()->set_id( pArmyKnowing_ ? pArmyKnowing_->GetID() : 0 );
    if( groupId_ )
        asn().mutable_knowledge_group()->set_id( *groupId_ );
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
    asn().mutable_knowledge()->set_id( nID_ );
    asn().mutable_object()->set_id( objectId_ );
    asn().mutable_party()->set_id( pArmyKnowing_ ? pArmyKnowing_->GetID() : 0 );
    if( pOwnerArmy_ )
        asn().mutable_object_party()->set_id( pOwnerArmy_->GetID() );
    if( !name_.empty() )
        asn().set_object_name( name_ );
    if( groupId_ )
        asn().mutable_knowledge_group()->set_id( *groupId_ );
    asn().mutable_type()->set_id( pObjectType_->GetName().c_str() );
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand viré dans le protocole ... le message de creation ne doit PAS envoyer les attributs
    if( locationType_ != TER_Localisation::eNone )
        asn().mutable_symbol()->set_type( static_cast< sword::Location_Geometry >( locationType_ ) );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SendMsgDestruction() const
{
    client::ObjectKnowledgeDestruction asn;
    asn().mutable_knowledge()->set_id( nID_ );
    asn().mutable_party()->set_id( pArmyKnowing_ ? pArmyKnowing_->GetID() : 0 );
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
double DEC_Knowledge_Object::GetMaxInteractionHeight() const
{
    if( const InteractionHeightAttribute* height = RetrieveAttribute< InteractionHeightAttribute >() )
        return height->Get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetName() const
// Created: JCR 2010-07-07
// -----------------------------------------------------------------------------
const std::string& DEC_Knowledge_Object::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::CopyFrom
// Created: JSR 2013-06-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::CopyFrom( const DEC_Knowledge_Object& object )
{
    pObjectType_ = object.pObjectType_;
    pOwnerArmy_ = object.pOwnerArmy_;
    localisation_ = object.localisation_;
    avoidanceLocalisation_ = object.avoidanceLocalisation_;
    pCurrentPerceptionLevel_ = object.pCurrentPerceptionLevel_;
    pPreviousPerceptionLevel_ = object.pPreviousPerceptionLevel_;
    pMaxPerceptionLevel_ = object.pMaxPerceptionLevel_;
    perceptionPerAutomateSet_ = object.perceptionPerAutomateSet_;
    previousPerceptionPerAutomateSet_ = object.previousPerceptionPerAutomateSet_;
    perceptionLevelPerAgentMap_ = object.perceptionLevelPerAgentMap_;
    nTimeLastUpdate_ = object.nTimeLastUpdate_;
    rRelevance_ = object.rRelevance_;
    bValid_ = object.bValid_;
    bPerceptionDistanceHacked_ = object.bPerceptionDistanceHacked_;
    bSkipPreparation_ = object.bSkipPreparation_;
    reconByAgentTypes_ = object.reconByAgentTypes_;
    UpdateAttributes( boost::bind( &DEC_Knowledge_IObjectAttributeProxy::CopyFrom, _1, boost::cref( object ) ) );
    nAttributesUpdated_ = eAttr_AllAttributes;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::Recon( const MIL_Agent_ABC& agent )
{
    bool inserted = reconByAgentTypes_.insert( &agent.GetType() ).second;
    if( !inserted || !IsValid() || !pObjectKnown_ )
        return;
    if( DEC_Knowledge_ObjectAttributeProxy_ABC< CrossingSiteAttribute >* proxy = Retrieve< DEC_Knowledge_ObjectAttributeProxy_ABC< CrossingSiteAttribute > >() )
        if( proxy->ForceUpdateAttributeFromObject( *pObjectKnown_ ) )
        {
            nAttributesUpdated_ = eAttr_AllAttributes;
            SendUpdateOnNetwork();
        }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsBypassed
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsBypassed() const
{
    if( const BypassAttribute* bypass = RetrieveAttribute< BypassAttribute >() )
        return bypass->IsBypassed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsMined
// Created: DDA 2012-03-16
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsMined() const
{
    if( const MineAttribute* attr = RetrieveAttribute< MineAttribute >() )
        return attr->GetState() > 0.;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsFullMined
// Created: MMC 2013-06-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsFullMined() const
{
    if( const MineAttribute* attr = RetrieveAttribute< MineAttribute >() )
        return attr->GetState() >= 1.;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsConstructed
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsConstructed() const
{
    if( const ConstructionAttribute* construction = RetrieveAttribute< ConstructionAttribute >() )
        return construction->IsConstructed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsActivableObstacle
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsActivableObstacle() const
{
    return pObjectType_ && pObjectType_->GetCapacity< ActivableCapacity >();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsObstacleActivated
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsObstacleActivated() const
{
    if( const ObstacleAttribute* activable = RetrieveAttribute< ObstacleAttribute >() )
        return activable->IsActivated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::MustBeMined
// Created: MMC 2013-06-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::MustBeMined() const
{
    if( !pObjectKnown_ )
        return false;
    if( const MineAttribute* attr = pObjectKnown_->RetrieveAttribute< MineAttribute >() )
        return attr->MustBeMined();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetActivationTime
// Created: GGE 2012-09-19
// -----------------------------------------------------------------------------
int DEC_Knowledge_Object::GetActivationTime() const
{
    if( const ObstacleAttribute* activable = RetrieveAttribute< ObstacleAttribute >() )
        return activable->GetActivationTime();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::CanCollideWith
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::CanCollideWith( const MIL_Agent_ABC& agent, bool bCheckBypassed /*= true*/ ) const
{
    bool bIsBypassed = bCheckBypassed ? IsBypassed() : false;
    return ( !IsActivableObstacle() || IsObstacleActivated() ) && !bIsBypassed && CanInteractWith( agent )
         && agent.GetRole< PHY_RoleInterface_Location >().GetHeight() <= GetMaxInteractionHeight();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::CanCollideWithEntity
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::CanCollideWithEntity() const
{
    return ( !IsActivableObstacle() || IsObstacleActivated() ) && !IsBypassed() && CanInteractWithEntity();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::CanInteractWith
// Created: LDC 2011-03-30
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    return !pObjectKnown_ || pObjectKnown_->CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::CanInteractWithEntity
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::CanInteractWithEntity() const
{
    return !pObjectKnown_ || pObjectKnown_->CanInteractWithEntity();
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
// Name: DEC_Knowledge_Object::GetObjectId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_Object::GetObjectId() const
{
    return objectId_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetArmy
// Created: NLD 2006-01-05
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_Object::GetArmy() const
{
    return pOwnerArmy_;
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
double DEC_Knowledge_Object::GetRelevance() const
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
// Name: DEC_Knowledge_Object::IsObjectInsidePathPoint
// Created: JSR 2010-12-22
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsObjectInsidePathPoint( const T_PointVector& pathPoints, const MIL_Agent_ABC* agent ) const
{
    static const double epsilon = 1e-8;
    if( IsOnBorder( pathPoints, epsilon ) )
        return false;
    if( agent && pObjectKnown_ )
        if( const FloodAttribute* flood = pObjectKnown_->RetrieveAttribute< FloodAttribute >() )
        {
            E_CrossingHeight crossingHeight = agent->GetType().GetUnitType().GetCrossingHeight();
            if( crossingHeight == eCrossingHeightAlways )
                return false;
            const TER_Localisation& localisation = flood->GetLocalisation();
            const auto& deepAreas = flood->GetDeepAreas();
            const auto& lowAreas = flood->GetLowAreas();
            for( auto it = pathPoints.begin(); it != pathPoints.end(); ++it )
                if( localisation.IsInside( *it, epsilon ) )
                {
                    geometry::Point2f point( static_cast< float>( ( *it ).rX_ ), static_cast< float >( ( *it ).rY_ ) );
                    for( auto polygonIt = deepAreas.begin(); polygonIt != deepAreas.end(); ++polygonIt )
                        if( polygonIt->BoundingBox().IsInside( point ) && polygonIt->IsInside( point ) )
                            return true;
                    if( crossingHeight == eCrossingHeightNever )
                        for( auto polygonIt = lowAreas.begin(); polygonIt != lowAreas.end(); ++polygonIt )
                            if( polygonIt->BoundingBox().IsInside( point ) && polygonIt->IsInside( point ) )
                                return true;
                }
            return false;
        }
    for( auto it = pathPoints.begin(); it != pathPoints.end(); ++it )
        if( localisation_.IsInside( *it, epsilon ) && ( !pObjectKnown_ || pObjectKnown_->GetLocalisation().IsInside( *it, epsilon ) ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsOnBorder
// Created: LDC 2014-01-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsOnBorder( const T_PointVector& pathPoints, double precision ) const
{
    for( auto it = pathPoints.begin(); it != pathPoints.end(); ++it )
        if( pObjectKnown_ && pObjectKnown_->GetLocalisation().IsOnBorder( *it, precision ) )
            return true;
    return false;
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
// Name: DEC_Knowledge_Object::CleanObjectKnown
// Created: JSR 2012-01-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::CleanObjectKnown()
{
    if( pObjectKnown_ && ( pObjectKnown_->IsMarkedForDestruction() || pObjectKnown_->IsMarkedForDestructionNextUpdate() ) )
    {
        if( bPerceptionDistanceHacked_ || pObjectKnown_->Retrieve< CrowdCapacity >() ) // $$$$ _RC_ LDC 2011:08:10 Should be pObjectKnown_->IsUniversal() and ADN modified accordingly.
            rRelevance_ = 0.;
        pObjectKnown_ = 0;
        NotifyAttributeUpdated( eAttr_RealObject );
    }
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
    CleanObjectKnown();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsAnEnemy
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
E_Tristate DEC_Knowledge_Object::IsAnEnemy( const MIL_Army_ABC& army ) const
{
    if( pOwnerArmy_ )
        return army.IsAnEnemy( *pOwnerArmy_ );
    return eTristate_False;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsAFriend
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
E_Tristate DEC_Knowledge_Object::IsAFriend( const MIL_Army_ABC& army ) const
{
    if( pOwnerArmy_ )
        return army.IsAFriend( *pOwnerArmy_ );
    return eTristate_False;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Object::GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const
{
    auto it = perceptionLevelPerAgentMap_.find( &pion );
    if( it != perceptionLevelPerAgentMap_.end() )
        return *it->second;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetCurrentPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Object::GetCurrentPerceptionLevel() const
{
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::HackPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel )
{
    if( *pPerceptionLevel > *pCurrentPerceptionLevel_ )
    {
        rRelevance_ = 1.0;
        bPerceptionDistanceHacked_ = true;
        bSkipPreparation_ = true;
        pCurrentPerceptionLevel_ = pPerceptionLevel;
        UpdateLocalisations();
        nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsPerceptionDistanceHacked
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::IsPerceptionDistanceHacked() const
{
    return bPerceptionDistanceHacked_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetMaxTrafficability
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
double DEC_Knowledge_Object::GetMaxTrafficability() const
{
    if( const TrafficabilityAttribute* pTrafficability = RetrieveAttribute< TrafficabilityAttribute >() )
    {
        const InteractWithSideCapacity* pSideInteraction = pObjectType_->GetCapacity< InteractWithSideCapacity >();
        bool interact = pObjectType_ && pSideInteraction;
        if( !interact || !pArmyKnowing_ || !pOwnerArmy_ || pSideInteraction->IsPossible( *pArmyKnowing_, *pOwnerArmy_ ) )
            return pTrafficability->GetMaxValue();
    }
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::HasAgentMaxSpeedMultiplier
// Created: LDC 2013-01-02
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Object::HasAgentMaxSpeedMultiplier() const
{
    const MobilityCapacity* mobility = GetType().GetCapacity< MobilityCapacity >();
    return mobility && mobility->IsMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::SkipPreparation
// Created: JSR 2012-02-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_Object::SkipPreparation()
{
    bSkipPreparation_ = true;
}