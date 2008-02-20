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

#include "dispatcher/Extension_ABC.h"

class ObjectIdentifier;
class AttributeIdentifier;
class Deserializer;
class UpdateFunctor_ABC;

namespace hla
{

// =============================================================================
/** @class  HlaExtension_ABC
    @brief  HlaExtension_ABC
*/
// Created: SBO 2008-02-18
// =============================================================================
class HlaExtension_ABC : public dispatcher::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HlaExtension_ABC();
    virtual ~HlaExtension_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Deserialize( const AttributeIdentifier& attributeID, const Deserializer& deserializer ) = 0;
    virtual void Serialize  ( UpdateFunctor_ABC& functor, bool bUpdateAll ) const = 0;
    //@}
};

}

#endif // __HlaExtension_ABC_h_
