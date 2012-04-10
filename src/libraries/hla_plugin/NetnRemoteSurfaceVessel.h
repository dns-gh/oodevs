// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRemoteSurfaceVessel_h
#define plugins_hla_NetnRemoteSurfaceVessel_h

#include "HlaObject_ABC.h"
#include <memory>

namespace plugins
{
namespace hla
{
    class AttributesDeserializer;
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  NetnRemoteSurfaceVessel
    @brief  Netn remote surface vessel
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnRemoteSurfaceVessel : public HlaObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnRemoteSurfaceVessel( std::auto_ptr< HlaObject_ABC > vessel, RemoteAgentListener_ABC& listener, const std::string& identifier );
    virtual ~NetnRemoteSurfaceVessel();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< HlaObject_ABC > vessel_;
    std::auto_ptr< AttributesDeserializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_NetnRemoteSurfaceVessel_h
