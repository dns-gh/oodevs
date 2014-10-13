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

#include "MSEllipse.h"
#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class CoordinateConverter_ABC;
    class Controller;
    class Profile_ABC;
}

namespace gui
{
    class GLView_ABC;
}

namespace weather
{
    class Meteo;
}

class Simulation;

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
    AmmoEffect( const sword::StartFireEffect& message, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::Agent_ABC >& agents, kernel::Profile_ABC& profile );
    virtual ~AmmoEffect();
    //@}

    //! @name Operations
    //@{
    void Draw( const gui::GLView_ABC& tools, const kernel::Profile_ABC& profile ) const;
    bool IsInside( const geometry::Point2f& point ) const;
    void ApplyEffect( weather::Meteo& meteo ) const;
    void SetPerceivers( const ::google::protobuf::RepeatedPtrField< ::sword::Id >& perceivers );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsVisible( const kernel::Profile_ABC& profile ) const;
    void PlaySoundEffect() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    std::vector< unsigned int > perceivers_;
    unsigned long id_;
    sword::StartFireEffect::EnumFireEffectType type_;
    MSEllipse ellipse_;
    kernel::Profile_ABC& profile_;
    //@}
};

#endif // __AmmoEffect_h_
