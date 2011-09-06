// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Aggregate_ABC_h
#define plugins_hla_Aggregate_ABC_h

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
/** @class  Aggregate_ABC
    @brief  Aggregate definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class Aggregate_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Aggregate_ABC() {}
    virtual ~Aggregate_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const = 0;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer ) = 0;
    //@}
};

}
}

#endif // plugins_hla_Aggregate_ABC_h
