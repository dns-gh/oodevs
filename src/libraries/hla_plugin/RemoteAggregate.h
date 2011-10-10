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

#include "HlaObject_ABC.h"
#include <memory>

namespace plugins
{
namespace hla
{
    class RemoteAgentListener_ABC;
    class AttributesDeserializer;

// =============================================================================
/** @class  RemoteAggregate
    @brief  Remote aggregate
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteAggregate : public HlaObject_ABC
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
    //! @name Member data
    //@{
    std::auto_ptr< AttributesDeserializer > attributes_;
    unsigned int numberOfSilentEntities_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAggregate_h
