// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_DebugFederateAmbassador_h
#define plugins_hla_DebugFederateAmbassador_h

#include <hla/FederateAmbassador_ABC.h>
#include <hla/Federate.h>

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{
    class ObjectResolver_ABC;

// =============================================================================
/** @class  DebugFederateAmbassador
    @brief  Debug federate ambassador implementation
*/
// Created: MCO 2009-01-26
// =============================================================================
class DebugFederateAmbassador : public ::hla::FederateAmbassador_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DebugFederateAmbassador( dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver );
    virtual ~DebugFederateAmbassador();
    //@}

    //! @name Object Management
    //@{
    virtual void DiscoverObjectInstance( const ::hla::ClassIdentifier& classID, const ::hla::ObjectIdentifier& objectID, const std::string& objectName );
    virtual void RemoveObjectInstance  ( const ::hla::ObjectIdentifier& objectID );
    virtual void ReflectAttributeValues( const ::hla::ObjectIdentifier& objectID, const T_Attributes& attributes );
    virtual void EnableRegistrationForObjectClass( const ::hla::ClassIdentifier& classID, bool enable );
    virtual void ProvideAttributeValueUpdate( const ::hla::ObjectIdentifier& objectID );

    virtual void TurnInteractions( const ::hla::InteractionIdentifier& interactionID, bool enable );
    virtual void ReceiveInteraction( const ::hla::InteractionIdentifier& interactionID, const T_Parameters& parameters );
    //@}

    //! @name Name Reservation Management
    //@{
    virtual void ReservationSucceded();
    virtual void ReservationFailed();
    //@}

    //! @name Time Management
    //@{
    virtual void TimeRegulationEnabled ( const ::hla::Time_ABC& time );
    virtual void TimeConstrainedEnabled( const ::hla::Time_ABC& time );
    virtual void TimeAdvanceGranted    ( const ::hla::Time_ABC& time );
    //@}

    //! @name Federation Management Services
    //@{
    virtual void AnnounceSynchronizationPoint( const std::string& label );
    virtual void FederationSynchronized( const std::string& label );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Logger_ABC& logger_;
    ObjectResolver_ABC& resolver_;
    std::string time_;
    //@}
};

}
}

#endif // plugins_hla_DebugFederateAmbassador_h
