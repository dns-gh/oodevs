// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAggregate_h
#define plugins_hla_RemoteAggregate_h

#include "Aggregate_ABC.h"
#include <map>
#include <boost/function.hpp>

namespace plugins
{
namespace hla
{
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  RemoteAggregate
    @brief  Remote aggregate
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteAggregate : public Aggregate_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAggregate( const std::string& identifier, RemoteAgentListener_ABC& listener );
    virtual ~RemoteAggregate();
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
    unsigned int numberOfSilentEntities_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAggregate_h
