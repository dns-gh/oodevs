// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnAircraft_h
#define plugins_hla_NetnAircraft_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include <memory>

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesSerializer;
    class ObjectListener_ABC;
    class ObjectListenerComposite;

// =============================================================================
/** @class  NetnAircraft
    @brief  Netn surface vessel
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnAircraft : public HlaObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnAircraft( std::auto_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& callsign, const std::string& uniqueIdentifier, const std::string& symbol );
    virtual ~NetnAircraft();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void SetIdentifier( const std::string& id );
    virtual const std::string& GetIdentifier() const;
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ObjectListenerComposite > listeners_;
    std::auto_ptr< HlaObject_ABC > aggregate_;
    std::auto_ptr< AttributesSerializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_NetnAircraft_h
