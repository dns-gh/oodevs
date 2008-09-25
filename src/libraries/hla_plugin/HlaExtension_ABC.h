// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HlaExtension_ABC_h_
#define __HlaExtension_ABC_h_

#include "clients_kernel/Extension_ABC.h"
#include <stdexcept>

namespace hla
{
    class AttributeIdentifier;
    class Deserializer;
    class UpdateFunctor_ABC;
}

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  HlaExtension_ABC
    @brief  HlaExtension_ABC
*/
// Created: SBO 2008-02-18
// =============================================================================
class HlaExtension_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HlaExtension_ABC() {}
    virtual ~HlaExtension_ABC() {}
    //@}

    //! @name Operations
    //@{
    void Deserialize( const ::hla::AttributeIdentifier& , const ::hla::Deserializer& )
    {
        throw std::runtime_error( __FUNCTION__ " not implemented" );
    }
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const = 0;
    //@}
};

}
}

#endif // __HlaExtension_ABC_h_
