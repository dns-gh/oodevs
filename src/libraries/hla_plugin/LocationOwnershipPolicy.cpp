// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "LocationOwnershipPolicy.h"
#include "RemoteAgentSubject_ABC.h"
#include "HLAClass_ABC.h"
#include "HLAObject_ABC.h"
#include "ObjectListener_ABC.h"
#include "OwnershipController_ABC.h"
#include "TransferSender_ABC.h"
#include "protocol/Protocol.h"
#include <hla/VariableLengthData.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    geometry::Polygon2d Convert( const sword::CoordLatLongList& message )
    {
        geometry::Polygon2d result;
        for( int i = 0; i < message.elem_size(); ++i )
            result.Add( geometry::Point2d( message.elem( i ).latitude(), message.elem( i ).longitude() ) );
        return result;
    }
}

// =============================================================================
/** @class  OwnershipState
    @brief  OwnershipState
*/
// Created: AHC 2012-02-21
// =============================================================================
struct LocationOwnershipPolicy::OwnershipState
    : private ObjectListener_ABC
    , private boost::noncopyable
{
public:
    typedef boost::function< bool(double,double) > LocationCheckFunctor;
    enum State
    {
        S_Local = 0,
        S_DivestPending,
        S_DivestSent,
        S_DivestRefused,
        S_Remote,
        S_AcquisitionPending,
        S_AcquisitionSent,
        S_AcquisitionRefused
    };
    OwnershipState( const LocationCheckFunctor& fctor, const std::string& agentID,
            HlaClass_ABC& hlaClass, HlaObject_ABC& object, bool isLocal,
            OwnershipController_ABC& ownershipController, TransferSender_ABC& transferSender );
    ~OwnershipState();

    void Divested();
    void Acquired();
    void Check();

private:
    //! @name Operations
    //@{
    void TransferCallback( bool accept );

    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
            unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
    virtual void UniqueIdChanged( const std::string& identifier, const std::vector< char >& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& units );
    virtual void GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type );
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children );
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
                int col, int lig, double xll, double yll, double dx, double dy );
    void DoCheck();
    //@}

private:
    LocationCheckFunctor isInDivestitureArea_;
    std::string agentID_;
    HlaClass_ABC& hlaClass_;
    HlaObject_ABC& object_;
    OwnershipController_ABC& ownershipController_;
    TransferSender_ABC& transferSender_;
    State state_;
    const bool localObject_;
    double latitude_, longitude_;
    ::hla::VariableLengthData tag_;
};

// -----------------------------------------------------------------------------
// Name: OwnershipState constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
LocationOwnershipPolicy::OwnershipState::OwnershipState( const LocationCheckFunctor& fctor, const std::string& agentID,
        HlaClass_ABC& hlaClass, HlaObject_ABC& object, bool isLocal,
        OwnershipController_ABC& ownershipController, TransferSender_ABC& transferSender )
    : isInDivestitureArea_( fctor )
    , agentID_( agentID )
    , hlaClass_( hlaClass )
    , object_( object )
    , ownershipController_( ownershipController )
    , transferSender_( transferSender )
    , state_( isLocal ? S_Local : S_Remote )
    , localObject_( isLocal )
{
    object_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OwnershipState destructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
LocationOwnershipPolicy::OwnershipState::~OwnershipState()
{
    object_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::DoCheck
// Created: AHC 2012-05-02
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::DoCheck()
{
    if( isInDivestitureArea_( latitude_, longitude_) )
    {
        switch( state_ )
        {
        case S_Local:
            state_ = S_DivestSent;
            transferSender_.RequestTransfer( agentID_, boost::bind( &LocationOwnershipPolicy::OwnershipState::TransferCallback, this, _1 ),
                    TransferSender_ABC::E_EntityPush, hlaClass_.GetAttributes(), tag_ );
            break;
        case S_AcquisitionPending:
        case S_AcquisitionSent:
        case S_AcquisitionRefused:
            state_ = S_Remote;
            break;
        default:
            // NOTHING
            break;
        };
    }
    else
    {
        if( localObject_ )
        {
            switch( state_ )
            {
            case S_Remote:
                    state_ = S_AcquisitionSent;
                    transferSender_.RequestTransfer( agentID_, boost::bind( &LocationOwnershipPolicy::OwnershipState::TransferCallback, this, _1 ),
                            TransferSender_ABC::E_EntityPull, hlaClass_.GetAttributes(), tag_ );
                break;
            case S_DivestPending:
            case S_DivestSent:
            case S_DivestRefused:
                state_ = S_Local;
                break;
            default:
                // NOTHING
                break;
            };
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::Check
// Created: AHC 2012-05-02
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::Check() {
    DoCheck();
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::Moved
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::Moved( const std::string& /*identifier*/, double latitude, double longitude )
{
    latitude_ = latitude;
    longitude_ = longitude;
    DoCheck();
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::TransferCallback
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::TransferCallback( bool accept )
{
    switch( state_ )
    {
    case S_DivestSent:
        if( accept )
        {
            state_ = S_DivestPending;
            ownershipController_.PerformDivestiture( agentID_, hlaClass_.GetAttributes(), tag_ );
        }
        else
        {
            state_ = S_DivestRefused;
        }
        break;
    case S_AcquisitionSent:
        if( accept )
        {
            state_ = S_AcquisitionPending;
            ownershipController_.PerformAcquisition( agentID_, hlaClass_.GetAttributes(), tag_ );
        }
        else
            state_ = S_AcquisitionRefused;
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::Divested
// Created: AHC 2012-03-02
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::Divested()
{
    state_ = S_Remote;
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::Acquired
// Created: AHC 2012-03-02
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::Acquired()
{
    state_ = S_Local;
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::SideChanged
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::NameChanged
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::TypeChanged
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::EquipmentUpdated
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
            unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::UniqueIdChanged
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::CallsignChanged
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::ParentChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    //  NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::SubAgregatesChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const std::set< std::string >& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::SubEntitiesChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::SubEntitiesChanged(const std::string& /*rtiIdentifier*/, const std::set< std::string >& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::OwnershipState::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::OwnershipState::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
LocationOwnershipPolicy::LocationOwnershipPolicy( tools::MessageController_ABC< sword::MessengerToClient_Content >& controller,
        OwnershipController_ABC& ownershipController, RemoteAgentSubject_ABC& subject, TransferSender_ABC& transferSender,
        const std::string divestitureZoneName )
    : divestitureZoneName_( divestitureZoneName )
    , ownershipController_( ownershipController )
    , transferSender_( transferSender )
    , subject_( subject )
{
    CONNECT( controller, *this, shape_creation);
    CONNECT( controller, *this, shape_destruction);
    subject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy destructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
LocationOwnershipPolicy::~LocationOwnershipPolicy()
{
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::IsInDivestureArea
// Created: AHC 2012-02-21
//----------------------------------------------
bool LocationOwnershipPolicy::IsInDivestureArea( double latitude, double longitude ) const
{
    const geometry::Point2d location( latitude, longitude );
    for( T_Areas::const_iterator it = area_.begin(); it != area_.end(); ++it )
        if( it->second.IsInside( location ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::Notify
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::Notify( const sword::ShapeCreation& message, int /*context*/ )
{
    const std::string shapeName=message.shape().category()+"/"+message.shape().pattern();
    if( shapeName == divestitureZoneName_ )
        area_[ message.id().id() ] = Convert( message.shape().points() );
    for( T_OwnershipStates::iterator it=states_.begin(); states_.end()!=it; ++it )
    {
        it->second->Check();
    }
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::Notify
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::Notify( const sword::ShapeDestruction& message, int /*context*/ )
{
    T_Areas::iterator it( area_.find( message.id().id() ) );
    if( area_.end() != it )
    {
        area_.erase( it );
        for( T_OwnershipStates::iterator it=states_.begin(); states_.end()!=it; ++it )
        {
            it->second->Check();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::RemoteCreated
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object )
{
    OwnershipState* ptr = new OwnershipState( boost::bind( &LocationOwnershipPolicy::IsInDivestureArea, this, _1, _2 ), identifier, hlaClass, object, false,
            ownershipController_, transferSender_ );
    states_[identifier] = OwnershipStatePtr(ptr);

}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::RemoteDestroyed
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::RemoteDestroyed( const std::string& identifier )
{
    states_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::LocalCreated
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object )
{
    assert( states_.end() == states_.find( identifier ) );
    OwnershipState* ptr = new OwnershipState( boost::bind( &LocationOwnershipPolicy::IsInDivestureArea, this, _1, _2 ), identifier, hlaClass, object, true,
            ownershipController_, transferSender_ );
    states_[identifier] = OwnershipStatePtr(ptr);
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::LocalDestroyed
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::LocalDestroyed( const std::string& identifier )
{
    states_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::Divested
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::Divested( const std::string& identifier, const T_AttributeIdentifiers& /*attributes*/ )
{
    T_OwnershipStates::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return ;
    it->second->Divested();
}

// -----------------------------------------------------------------------------
// Name: LocationOwnershipPolicy::Acquired
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void LocationOwnershipPolicy::Acquired( const std::string& identifier, const T_AttributeIdentifiers& /*attributes*/ )
{
    T_OwnershipStates::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return;
    it->second->Acquired();
}

