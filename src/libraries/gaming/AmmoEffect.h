// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AmmoEffect_h_
#define __AmmoEffect_h_

#include "ASN_Types.h"
#include "MSEllipse.h"

namespace kernel
{
    class GlTools_ABC;
    class CoordinateConverter_ABC;
    class Controller;
}

// =============================================================================
/** @class  AmmoEffect
    @brief  AmmoEffect
*/
// Created: AGE 2006-04-04
// =============================================================================
class AmmoEffect
{

public:
    //! @name Constructors/Destructor
    //@{
             AmmoEffect( const ASN1T_MsgStartFireEffect& message, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AmmoEffect();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AmmoEffect( const AmmoEffect& );            //!< Copy constructor
    AmmoEffect& operator=( const AmmoEffect& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned long id_;
    ASN1T_EnumFireEffectType type_;
    MSEllipse ellipse_;
    //@}
};

#endif // __AmmoEffect_h_
