// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_LocationOwnershipPolicy_h_
#define plugins_hla_LocationOwnershipPolicy_h_

#include "OwnershipPolicy_ABC.h"
#include "ClassListener_ABC.h"
#include "tools/MessageObserver.h"
#include <geometry/Types.h>
#include <map>

namespace sword
{
    class MessengerToClient_Content;
    class ShapeCreation;
    class ShapeDestruction;
}

namespace plugins
{
namespace hla
{
    class OwnershipArea_ABC;
    class RemoteAgentSubject_ABC;
    class OwnershipController_ABC;
    class TransferSender_ABC;

// =============================================================================
/** @class  LocationOwnershipPolicy
    @brief  LocationOwnershipPolicy
*/
// Created: AHC 2012-02-21
// =============================================================================
class LocationOwnershipPolicy : public OwnershipPolicy_ABC
                                , private tools::MessageObserver< sword::ShapeCreation >
                                , private tools::MessageObserver< sword::ShapeDestruction >
                                , private ClassListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationOwnershipPolicy( tools::MessageController_ABC< sword::MessengerToClient_Content >& controller,
            OwnershipController_ABC& ownershipController, RemoteAgentSubject_ABC& subject,
            TransferSender_ABC& transferSender, const std::string divestitureZoneName );
    virtual ~LocationOwnershipPolicy();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInDivestureArea( double latitude, double longitude ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ShapeCreation& message, int context );
    virtual void Notify( const sword::ShapeDestruction& message, int context );
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, geometry::Polygon2d > T_Areas;
    struct OwnershipState;
    typedef boost::shared_ptr< OwnershipState > OwnershipStatePtr;
    typedef std::map< std::string, OwnershipStatePtr >  T_OwnershipStates;
    //@}

private:
    //! @name Member data
    //@{
    const std::string divestitureZoneName_;
    RemoteAgentSubject_ABC& subject_;
    OwnershipController_ABC& ownershipController_;
    TransferSender_ABC& transferSender_;
    T_Areas area_;
    T_OwnershipStates states_;
    //@}
};
}
}

#endif // plugins_hla_LocationOwnershipPolicy_h_
