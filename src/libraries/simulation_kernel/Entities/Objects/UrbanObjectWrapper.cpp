// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanObjectWrapper.h"
#include <urban/TerrainObject_ABC.h>
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_ObjectManipulator.h"

#include "protocol/clientsenders.h"

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper( urban::TerrainObject_ABC& object )
    : object_( object )
    , manipulator_( *new MIL_ObjectManipulator( *this ) )
{
    
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper destructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::~UrbanObjectWrapper() 
{
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::WriteODB
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::WriteODB( xml::xostream& xos ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( MIL_InteractiveContainer_ABC* capacity )
{

}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentEntering
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentExiting
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentMovingInside
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentInside
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessPopulationInside
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessPopulationInside( MIL_PopulationElement_ABC& population )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_Army_ABC& team )
{
    return boost::shared_ptr< DEC_Knowledge_Object >( new DEC_Knowledge_Object() );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_KnowledgeGroup& group )
{
    return boost::shared_ptr< DEC_Knowledge_Object >( new DEC_Knowledge_Object() );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::operator()
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
const MIL_ObjectManipulator_ABC& UrbanObjectWrapper::operator()() const
{
    return manipulator_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::operator()
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
MIL_ObjectManipulator_ABC& UrbanObjectWrapper::operator()()
{
    return manipulator_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetHLAView
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
HLA_Object_ABC* UrbanObjectWrapper::GetHLAView() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SetHLAView
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SetHLAView( HLA_Object_ABC& view )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Deserialize
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Serialize
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Serialize( HLA_UpdateFunctor& functor ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnUpdate
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdate( const Common::MsgMissionParameter_Value& asn )
{
    return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendCreation
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendCreation() const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendDestruction
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendDestruction() const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendFullState
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendFullState() const
{
    //NOTHING
}
  
// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetID
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetID() const
{
    return object_.GetId();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetObject
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
urban::TerrainObject_ABC& UrbanObjectWrapper::GetObject()
{
    return object_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Update
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Update( unsigned int time )
{

}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( ObjectAttribute_ABC* attribute )
{

}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( ObjectCapacity_ABC* capacity )
{

}


#if 0
// -----------------------------------------------------------------------------
// Name: Object::GetID
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
unsigned int Object::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Object::load
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    file >> name_
         >> id_;
    
    T_Capacities capacities;
    file >> capacities;
    std::for_each( capacities.begin(), capacities.end(), boost::bind( &ObjectCapacity_ABC::Register, _1, boost::ref( *this ) ) );

    T_Attributes attributes;
    file >> attributes;
    std::for_each( attributes.begin(), attributes.end(), boost::bind( &ObjectAttribute_ABC::Register, _1, boost::ref( *this ) ) );

    idManager_.Lock( id_ );
    //MIL_Object_ABC::Register();
}
    

// -----------------------------------------------------------------------------
// Name: Object::save
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    file << name_;
    file << id_;
    file << capacities_;
    file << attributes_;
}

// -----------------------------------------------------------------------------
// Name: Object::WriteODB
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "object" )
            << xml::attribute( "id"  , id_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", GetType().GetName() );
    
    GetLocalisation().Write( xos );

    xos << xml::start( "attributes" );
    {
        std::for_each( attributes_.begin(), attributes_.end(), 
            boost::bind( &ObjectAttribute_ABC::WriteODB, _1, boost::ref( xos ) ) );
    }
    xos << xml::end();
    xos << xml::end(); // object
}

// -----------------------------------------------------------------------------
// Name: Object::AddAttribute
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::Register( ObjectAttribute_ABC* attribute )
{    
    attributes_.push_back( attribute );
}

// -----------------------------------------------------------------------------
// Name: Object::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::Register( ObjectCapacity_ABC* capacity )
{
    capacities_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: Object::RegisterInteractive
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::Register( MIL_InteractiveContainer_ABC* capacity )
{
    interactives_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: Object::Instanciate
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
void Object::Instanciate( Object& object ) const
{    
    std::for_each( capacities_.begin(), capacities_.end(), 
                   boost::bind( &ObjectCapacity_ABC::Instanciate, _1, boost::ref( object ) ) );
//    std::for_each( object.capacities_.begin(), object.capacities_.end(), 
//                   boost::bind( &ObjectCapacity_ABC::Register, _1, boost::ref( object ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Finalize
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
void Object::Finalize()
{    
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Finalize, _1, boost::ref( *this ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Update
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
void Object::Update( unsigned int time )
{
    // TODO can be updated
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Update, _1, boost::ref( *this ), time ) );
    const ConstructionAttribute* attribute = RetrieveAttribute< ConstructionAttribute >();
    if( attribute && attribute->NeedDestruction() )
        MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: Object::CanInteractWith
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
bool Object::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    bool canInteract = true;
    std::for_each( interactives_.begin(), interactives_.end(), boost::bind( &MIL_InteractiveContainer_ABC::CanInteractWith, _1, boost::ref( *this ), boost::ref( agent ), boost::ref( canInteract ) ) );
    return canInteract && MIL_Object_ABC::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentEntering
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentEntering, _1, boost::ref( *this ), boost::ref( agent ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentExiting
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentExiting, _1, boost::ref( *this ), boost::ref( agent ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentMovingInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyTerrainObjectCollision( *this );
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentMovingInside, _1, boost::ref( *this ), boost::ref( agent ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyTerrainObjectCollision( *this );
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentInside, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::ProcessPopulationInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void Object::ProcessPopulationInside( MIL_PopulationElement_ABC& population )
{
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessPopulationInside, _1, boost::ref( *this ), boost::ref( population ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::operator()
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_ObjectManipulator_ABC& Object::operator()()
{
    return *manipulator_;
}

// -----------------------------------------------------------------------------
// Name: Object::operator()
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
const MIL_ObjectManipulator_ABC& Object::operator()() const
{
    return *manipulator_;
}

// -----------------------------------------------------------------------------
// Name: Object::OnUpdate
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode Object::OnUpdate( const MsgMissionParameter_Value& attributes )
{
    for( int i = 0; i < attributes.list_size(); ++i )
    {
        const MsgMissionParameter_Value& attribute = attributes.list( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_specific_attributes;

        unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type

        switch( actionId )
        {
        case MsgObjectMagicAction_Attribute_mine:
            GetAttribute< MineAttribute >().OnUpdate( attribute );
            break;
        case MsgObjectMagicAction_Attribute_bypass:
            GetAttribute< BypassAttribute >().OnUpdate( attribute );
            break;
        case MsgObjectMagicAction_Attribute_construction:
            GetAttribute< ConstructionAttribute >().OnUpdate( attribute );
            break;
        case MsgObjectMagicAction_Attribute_obstacle:
            GetAttribute< ObstacleAttribute >().OnUpdate( attribute );
            break;
        case MsgObjectMagicAction_Attribute_crossing_site:
            GetAttribute< CrossingSiteAttribute >().OnUpdate( attribute );
            break;
        case MsgObjectMagicAction_Attribute_supply_route:
            GetAttribute< SupplyRouteAttribute >().OnUpdate( attribute );
            break;
        default:
            break;
        }
    }

    return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error;
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendCreation() const
{
    if( pView_ && pView_->HideObject() )
        return;

    client::ObjectCreation asn;

    asn().set_oid( GetID() );
    asn().set_name( name_ );
    asn().set_type( GetType().GetName() );
    asn().set_team( GetArmy()->GetID() );

    NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    std::for_each( attributes_.begin(), attributes_.end(), 
                    boost::bind( &ObjectAttribute_ABC::SendFullState, _1, boost::ref( *asn().mutable_attributes() ) ) );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendDestruction() const
{
    if( pView_ && pView_->HideObject() )
        return;

    client::ObjectDestruction asn;
    asn().set_oid( GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void Object::UpdateState()
{
    SendMsgUpdate();
    MIL_Object_ABC::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullState
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendFullState() const
{
//    NotifyAttributeUpdated( eAttrUpdate_All );
    xAttrToUpdate_ |= eAttrUpdate_Localisation;
    SendMsgUpdate();
}

// -----------------------------------------------------------------------------
// Name: Object::Object::SendMsgUpdate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::SendMsgUpdate() const
{
    if( pView_ && pView_->HideObject() )
        return;

    client::ObjectUpdate asn;
    asn().set_oid( id_ );

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::SendUpdate, _1, boost::ref( *asn().mutable_attributes() ) ) );

    if( xAttrToUpdate_ & eAttrUpdate_Localisation )
        NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    
    Common::MsgObjectAttributes& attr = *asn().mutable_attributes();

    if ( asn().has_location() || attr.has_construction() || attr.has_obstacle() 
        || attr.has_mine() || attr.has_activity_time() || attr.has_bypass() 
        || attr.has_logistic() || attr.has_nbc() || attr.has_crossing_site()
        || attr.has_supply_route() || attr.has_toxic_cloud() || attr.has_fire()
        || attr.has_medical_treatment() || attr.has_interaction_height() || attr.has_stock()
        || attr.has_nbc_agent() )
        asn.Send( NET_Publisher_ABC::Publisher() );
    
    xAttrToUpdate_ = 0;

    if( asn().has_location() )
        asn().mutable_location()->Clear();
    asn().mutable_attributes()->Clear();
}

// -----------------------------------------------------------------------------
// Name: Object::GetHLAView
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_Object_ABC* Object::GetHLAView() const
{
    return pView_;
}

// -----------------------------------------------------------------------------
// Name: Object::SetView
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::SetHLAView( HLA_Object_ABC& view )
{
    delete pView_;
    pView_ = &view;
}

// -----------------------------------------------------------------------------
// Name: Object::Deserialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "coordonnees" )
    {
        TER_Localisation newLocalisation;
        deserializer >> newLocalisation;
        UpdateLocalisation( newLocalisation );
    }
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Deserialize, _1, boost::cref( attributeID ), deserializer ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Serialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "armee", false, GetArmy()->GetName() );
    functor.Serialize( "type",  false, GetType().GetName() );
    functor.Serialize( "coordonnees",   ( xAttrToUpdateForHLA_ & eAttrUpdate_Localisation ) != 0,           GetLocalisation() );
    
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Serialize, _1, boost::ref( functor ) ) );

    xAttrToUpdateForHLA_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Object::CreateKnowledge
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > Object::CreateKnowledge( const MIL_Army_ABC& team )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge( new DEC_Knowledge_Object( team, *this ) );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Instanciate, _1, boost::ref( *pKnowledge ) ) );
    return pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > Object::CreateKnowledge
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > Object::CreateKnowledge( const MIL_KnowledgeGroup& group )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge( new DEC_Knowledge_Object( group, *this ) );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Instanciate, _1, boost::ref( *pKnowledge ) ) );
    return pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateLocatisation
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void Object::UpdateLocalisation( const TER_Localisation& location )
{
    xAttrToUpdate_ |= eAttrUpdate_Localisation;    
    MIL_Object_ABC::UpdateLocalisation( location );
}

#endif