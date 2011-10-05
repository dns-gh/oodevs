// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteSurfaceVessel_h
#define plugins_hla_RemoteSurfaceVessel_h

#include "HlaObject_ABC.h"
#include <map>
#include <boost/function.hpp>

namespace plugins
{
namespace hla
{
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  RemoteSurfaceVessel
    @brief  Remote surface vessel
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteSurfaceVessel : public HlaObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteSurfaceVessel( const std::string& identifier, RemoteAgentListener_ABC& listener );
    virtual ~RemoteSurfaceVessel();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::function< void( ::hla::Deserializer&, const std::string&, RemoteAgentListener_ABC& ) > T_Notification;
    typedef std::map< std::string, T_Notification > T_Notifications;
    //@}

private:
    //! @name Member data
    //@{
    const std::string identifier_;
    RemoteAgentListener_ABC& listener_;
    const T_Notifications notifications_;
    //@}
};

}
}

#endif // plugins_hla_RemoteSurfaceVessel_h
