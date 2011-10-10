// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnSurfaceVessel_h
#define plugins_hla_NetnSurfaceVessel_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include <memory>

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesSerializer;

// =============================================================================
/** @class  NetnSurfaceVessel
    @brief  Netn surface vessel
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnSurfaceVessel : public HlaObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnSurfaceVessel( std::auto_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& callsign, const std::string& uniqueIdentifier );
    virtual ~NetnSurfaceVessel();
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
    std::auto_ptr< AttributesSerializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_NetnSurfaceVessel_h
