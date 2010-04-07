// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AmmoEffect_h_
#define __AmmoEffect_h_

#include "MSEllipse.h"
#include "protocol/Protocol.h"
#include "meteo/PHY_Meteo.h"

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
             AmmoEffect( const MsgsSimToClient::MsgStartFireEffect& message, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AmmoEffect();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual const weather::PHY_Meteo& GetMeteo() const;
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
    Common::EnumFireEffectType type_;
    MSEllipse ellipse_;
    weather::PHY_Meteo meteo_;
    //@}
};

#endif // __AmmoEffect_h_
