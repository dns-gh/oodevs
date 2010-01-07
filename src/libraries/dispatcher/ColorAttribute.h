// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ColorAttribute_h_
#define __ColorAttribute_h_

#include "game_asn/Simulation.h"
#include "UrbanObjectAttribute_ABC.h"

class ColorRGBA;

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ColorAttribute
    @brief  ColorAttribute
*/
// Created: RPD 2010-01-06
// =============================================================================
class ColorAttribute : public UrbanObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorAttribute( const Model& model, const ASN1T_UrbanAttributes& asnMsg );
    virtual ~ColorAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_UrbanAttributes& asnMsg );
    virtual void Send     ( ASN1T_UrbanAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_UrbanAttributes& asnMsg ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ColorAttribute( const ColorAttribute& );            //!< Copy constructor
    ColorAttribute& operator=( const ColorAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ColorRGBA* color_;
    //@}
};

}

#endif // __ColorAttribute_h_
