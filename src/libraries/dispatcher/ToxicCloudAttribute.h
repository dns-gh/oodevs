 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ToxicCloudAttribute_h_
#define __ToxicCloudAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

struct ASN1T_LocatedQuantityList;

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ToxicCloudAttribute
    @brief  ToxicCloudAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ToxicCloudAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ToxicCloudAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~ToxicCloudAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}
private:
    //! @name 
    //@{
    void Clear();
    //@}

private:
    //! @name Data member
    //@{
    ASN1T_LocatedQuantityList  quantities_;
    //@}
};

}

#endif // __ToxicCloudAttribute_h_
