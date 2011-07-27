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

#include <boost/noncopyable.hpp>
#include "MSEllipse.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class GlTools_ABC;
    class CoordinateConverter_ABC;
    class Controller;
}

namespace weather
{
    class Meteo;
}

// =============================================================================
/** @class  AmmoEffect
    @brief  AmmoEffect
*/
// Created: AGE 2006-04-04
// =============================================================================
class AmmoEffect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AmmoEffect( const sword::StartFireEffect& message, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AmmoEffect();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    void ApplyEffect( weather::Meteo& meteo ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned long id_;
    sword::StartFireEffect::EnumFireEffectType type_;
    MSEllipse ellipse_;
    //@}
};

#endif // __AmmoEffect_h_
