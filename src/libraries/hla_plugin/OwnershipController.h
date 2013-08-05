// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_OwnershipController_h
#define plugins_hla_OwnershipController_h

#include "ClassListener_ABC.h"
#include "OwnershipController_ABC.h"
#include "rpr/EntityIdentifier.h"
#include <hla/InteractionNotification_ABC.h>
#include <memory>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{

class RemoteAgentSubject_ABC;
class Federate_ABC;
class OwnershipStrategy_ABC;
class InteractionBuilder;
class LocalAgentResolver_ABC;
class ContextFactory_ABC;
template< typename Interaction > class InteractionSender_ABC;

namespace interactions
{
    struct TransferControl;
}

// =============================================================================
/** @class  OwnershipController
    @brief  OwnershipController
*/
// Created: AHC 2012-02-21
// =============================================================================
class OwnershipController : public OwnershipController_ABC
                            , private ClassListener_ABC
                            , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    OwnershipController( const rpr::EntityIdentifier& federateID, RemoteAgentSubject_ABC& subject,
        dispatcher::Logger_ABC& logger );
    ~OwnershipController();
    //@}

    //! @name Operations
    //@{
    virtual void PerformDivestiture( const std::string& identifier, const std::vector< ::hla::AttributeIdentifier>& attributes  );
    virtual void PerformAcquisition( const std::string& identifier, const std::vector< ::hla::AttributeIdentifier>& attributes  );
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    //@}

private:
    //! @name Types definitions
    //@{
    struct OwnershipInfo;
    typedef boost::shared_ptr< OwnershipInfo > OwnershipInfoPtr;
    typedef std::map< std::string, OwnershipInfoPtr >  T_OwnershipInfos;
    //@}

private:
    //! @name Member data
    //@{
    const rpr::EntityIdentifier federateID_;
    RemoteAgentSubject_ABC& subject_;
    dispatcher::Logger_ABC& logger_;
    T_OwnershipInfos states_;
    //@}
};

}
}

#endif // plugins_hla_OwnershipController_h
