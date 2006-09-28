 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaObjectAttribute_h_
#define __RotaObjectAttribute_h_

#include "AsnTypes.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  RotaObjectAttribute
    @brief  RotaObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class RotaObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     RotaObjectAttribute( const Model& model, const ASN1T_AttrObjectSpecific& asnMsg );
    ~RotaObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_AttrObjectSpecific& asnMsg );
    virtual void Send     ( ASN1T_AttrObjectSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_AttrObjectSpecific& asnMsg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< unsigned int > T_IDVector;
    //@}

private:
    unsigned int nDanger_;
    T_IDVector   nbcAgents_;
};

}

#endif // __RotaObjectAttribute_h_
