 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimeAttribute_h_
#define __ActivityTimeAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ActivityTimeAttribute
    @brief  ActivityTimeAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ActivityTimeAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ActivityTimeAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~ActivityTimeAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{    
    unsigned int nActivityTime_;    
    //@}
    
};

}

#endif // __ActivityTimeAttribute_h_
