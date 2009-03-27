 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttribute_h_
#define __CrossingSiteAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  CrossingSiteAttribute
    @brief  CrossingSiteAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class CrossingSiteAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     CrossingSiteAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~CrossingSiteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    unsigned int nWidth_;
    unsigned int nDepth_;
    unsigned int nSpeed_;
    bool         bNeedsConstruction_;    
};

}

#endif // __CrossingSiteAttribute_h_
