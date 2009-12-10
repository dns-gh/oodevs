// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VegetationAttribute_h_
#define __VegetationAttribute_h_

#include "game_asn/Simulation.h"
#include "UrbanObjectAttribute_ABC.h"

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  VegetationAttribute
@brief  VegetationAttribute
*/
// Created: SLG 2009-12-04
// =============================================================================
class VegetationAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    VegetationAttribute( const Model& model, const ASN1T_UrbanAttributes& asnMsg );
    ~VegetationAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_UrbanAttributes& asnMsg );
    virtual void Send     ( ASN1T_UrbanAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_UrbanAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    std::string     type_;
    unsigned int    height_;
    float           density_;
    //@}
};

}

#endif // __VegetationAttribute_h_
