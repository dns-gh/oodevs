// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_DebugRtiAmbassador_h
#define plugins_hla_DebugRtiAmbassador_h

#include <hla/RtiAmbassador_ABC.h>
#include <memory>

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
/** @class  DebugRtiAmbassador
    @brief  Debug RTI ambassador implementation
*/
// Created: MCO 2009-01-26
// =============================================================================
class DebugRtiAmbassador : public ::hla::RtiAmbassador_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DebugRtiAmbassador( std::auto_ptr< ::hla::RtiAmbassador_ABC > ambassador, dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver );
    virtual ~DebugRtiAmbassador();
    //@}

    //! @name Operations
    //@{
    virtual void Tick();
    //@}

    //! @name Federation Management
    //@{
    virtual bool Connect( ::hla::FederateAmbassador_ABC& ambassador );
    virtual void Disconnect();

    virtual bool Create( const std::string& federation, const std::string& fomFile );
    virtual bool Create( const std::string& federation, const T_FomFiles& fomFiles );
    virtual bool Destroy( const std::string& federation );

    virtual bool Join  ( const std::string& federate, const std::string& federation, ::hla::FederateAmbassador_ABC& ambassador );
    virtual void Resign();
    //@}

    //! @name Declaration Management
    //@{
    virtual void Subscribe( const ::hla::ClassIdentifier& classID, const ::hla::Class_ABC& objectClass );
    virtual void Publish  ( const ::hla::ClassIdentifier& classID, const ::hla::Class_ABC& objectClass );

    virtual void Subscribe( const ::hla::InteractionIdentifier& interactionID );
    virtual void Publish  ( const ::hla::InteractionIdentifier& interactionID );
    //@}

    //! @name Object Management
    //@{
    virtual void                  ReserveObjectInstance( const std::string& name, ::hla::FederateAmbassador_ABC& ambassador );
    virtual ::hla::ObjectIdentifier RegisterObjectInstance( const ::hla::ClassIdentifier& classID, const std::string& name );
    virtual void                  DeleteObjectInstance  ( const ::hla::ObjectIdentifier& objectID );

    virtual void UpdateAttributeValues( const ::hla::ObjectIdentifier& objectID, const T_Attributes& attributes, const ::hla::Time_ABC& time );

    virtual void RequestObjectAttributeValueUpdate( const ::hla::ObjectIdentifier& objectID, const ::hla::Class_ABC& objectClass );

    virtual void SendInteraction( const ::hla::InteractionIdentifier& interactionID, const T_Parameters& parameters, const ::hla::Time_ABC& time );
    //@}

    //! @name Time Management
    //@{
    virtual void EnableTimeConstrained();
    virtual void EnableTimeRegulation( const ::hla::TimeInterval_ABC& lookAhead );

    virtual void NextEventRequestAvailable( const ::hla::Time_ABC& target );
    //@}

    //! @name Synchronization points
    //@{
    virtual void SynchronizationPointAchieved( const std::string& label );
    virtual void RegisterSynchronizationPoint( const std::string& label );
    //@}

private:
    //! @name Helpers
    //@{
    void Flush();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::RtiAmbassador_ABC > ambassador_;
    dispatcher::Logger_ABC& logger_;
    ObjectResolver_ABC& resolver_;
    bool ticking_;
    std::string time_;
    //@}
};

}
}

#endif // plugins_hla_DebugRtiAmbassador_h
