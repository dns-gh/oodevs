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
    class ObjectListener_ABC;
    class ObjectListenerComposite;

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
             NetnRemoteAggregate( std::auto_ptr< HlaObject_ABC > aggregate, const std::string& identifier );
    virtual ~NetnRemoteAggregate();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void SetIdentifier( const std::string& id );
    const std::string& GetIdentifier( ) const;
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ObjectListenerComposite > listeners_;
    std::auto_ptr< HlaObject_ABC > aggregate_;
    std::auto_ptr< AttributesDeserializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_NetnRemoteAggregate_h
