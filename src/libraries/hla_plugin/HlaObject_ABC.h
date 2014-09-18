// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HlaObject_ABC_h
#define plugins_hla_HlaObject_ABC_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace hla
{
    class UpdateFunctor_ABC;
    class AttributeIdentifier;
    class Deserializer_ABC;
}

namespace plugins
{
namespace hla
{
class ObjectListener_ABC;
class Agent_ABC;

// =============================================================================
/** @class  HlaObject_ABC
    @brief  Hla object definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class HlaObject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaObject_ABC() {}
    virtual ~HlaObject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const = 0;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer ) = 0;
    virtual void Register( ObjectListener_ABC& listener ) = 0;
    virtual void Unregister( ObjectListener_ABC& listener ) = 0;
    virtual void Attach( Agent_ABC* agent, unsigned long simId ) = 0;
    virtual Agent_ABC* const GetAgent() const = 0;
    virtual const std::string& GetIdentifier() const = 0;
    virtual void ResetAttributes() = 0;
    //@}
};

}
}

#endif // plugins_hla_HlaObject_ABC_h
