 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticRouteObjectAttribute_h_
#define __LogisticRouteObjectAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  LogisticRouteObjectAttribute
    @brief  LogisticRouteObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogisticRouteObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticRouteObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual ~LogisticRouteObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    //@}

private:
    bool         bEquipped_;
    unsigned int nMaxWeight_;
    unsigned int nWidth_;
    unsigned int nLength_;
    unsigned int nFlow_;
};

}

#endif // __LogisticRouteObjectAttribute_h_
