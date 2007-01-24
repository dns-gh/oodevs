 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCObjectAttribute_h_
#define __NBCObjectAttribute_h_

#include "network/Simulation_Asn.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  NBCObjectAttribute
    @brief  NBCObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class NBCObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     NBCObjectAttribute( const Model& model, const ASN1T_AttrObjectSpecific& asnMsg );
    ~NBCObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_AttrObjectSpecific& asnMsg );
    virtual void Send     ( ASN1T_AttrObjectSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_AttrObjectSpecific& asnMsg ) const;
    //@}

private:
    unsigned int nNBCAgentType_; // XML reference - no resolved by dispatcher

};

}

#endif // __NBCObjectAttribute_h_
