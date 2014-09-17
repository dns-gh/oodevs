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
#include "UnicodeString.h"
#include "UniqueId.h"
#include <memory>

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesUpdater;
    class ObjectListener_ABC;
    class ObjectListenerComposite;
    class FOM_Serializer_ABC;

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
             NetnSurfaceVessel( std::unique_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& callsign,
                 const std::vector< char >& uniqueIdentifier, const std::string& symbol, FOM_Serializer_ABC& fomSerializer, const std::string& rtiId );
             NetnSurfaceVessel( std::unique_ptr< HlaObject_ABC > vessel, const std::string& identifier, FOM_Serializer_ABC& fomSerializer );
    virtual ~NetnSurfaceVessel();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual Agent_ABC* const GetAgent() const;
    virtual const std::string& GetIdentifier() const;
    virtual void ResetAttributes();
    //@}

private:
    //! @name Operations
    //@{
    void RegisterAttributes();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< ObjectListenerComposite > listeners_;
    std::unique_ptr< HlaObject_ABC > aggregate_;
    FOM_Serializer_ABC& fomSerializer_;
    std::unique_ptr< AttributesUpdater > attributesUpdater_;
    UnicodeString callsign_;
    std::vector< char > uniqueId_;
    std::string identifier_;
    //@}
};

}
}

#endif // plugins_hla_NetnSurfaceVessel_h
