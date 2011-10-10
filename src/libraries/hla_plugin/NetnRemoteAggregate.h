// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRemoteAggregate_h
#define plugins_hla_NetnRemoteAggregate_h

#include "HlaObject_ABC.h"
#include <memory>

namespace plugins
{
namespace hla
{
    class AttributesDeserializer;
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  NetnRemoteAggregate
    @brief  Netn remote aggregate
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnRemoteAggregate : public HlaObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnRemoteAggregate( std::auto_ptr< HlaObject_ABC > aggregate, RemoteAgentListener_ABC& listener, const std::string& identifier );
    virtual ~NetnRemoteAggregate();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< HlaObject_ABC > aggregate_;
    std::auto_ptr< AttributesDeserializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_NetnRemoteAggregate_h
