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
#include <memory>

namespace plugins
{
namespace hla
{
    class ObjectListener_ABC;
    class AttributesDeserializer;
    class ObjectListenerComposite;

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
             RemoteSurfaceVessel( const std::string& identifier );
    virtual ~RemoteSurfaceVessel();
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
    std::string identifier_;
    std::auto_ptr< ObjectListenerComposite > listeners_;
    std::auto_ptr< AttributesDeserializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_RemoteSurfaceVessel_h
