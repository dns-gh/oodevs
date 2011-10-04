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

namespace hla
{
    class UpdateFunctor_ABC;
    class AttributeIdentifier;
    class Deserializer;
}

namespace plugins
{
namespace hla
{
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
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer ) = 0;
    //@}
};

}
}

#endif // plugins_hla_HlaObject_ABC_h
