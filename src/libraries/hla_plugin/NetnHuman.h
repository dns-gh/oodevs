// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnHuman_h
#define plugins_hla_NetnHuman_h

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
    class FOM_Serializer_ABC;

// =============================================================================
/** @class  NetnHuman
    @brief  Netn NetnHuman
*/
    // Created: AHC 2012-11-12
// =============================================================================
class NetnHuman : public HlaObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
        NetnHuman( std::unique_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& callsign,
                 const std::vector< char >& uniqueIdentifier, const std::string& symbol, FOM_Serializer_ABC& fomSerializer, const std::string& rtiId );
    virtual ~NetnHuman( );
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual const std::string& GetIdentifier() const;
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual Agent_ABC* const GetAgent() const;
    virtual void ResetAttributes();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< ObjectListenerComposite > listeners_;
    std::unique_ptr< HlaObject_ABC > aggregate_;
    FOM_Serializer_ABC& fomSerializer_;
    std::unique_ptr< AttributesSerializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_NetnHuman_h
