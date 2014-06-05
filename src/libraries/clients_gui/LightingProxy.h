// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LightingProxy_h_
#define __LightingProxy_h_

#include <graphics/Lighting_ABC.h>

class FixedLighting;
class TimeLighting;

namespace gui
{

// =============================================================================
/** @class  LightingProxy
    @brief  Lighting proxy
*/
// Created: AGE 2007-02-23
// =============================================================================
class LightingProxy : public QObject
                    , public Lighting_ABC

{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LightingProxy( QObject* parent );
    virtual ~LightingProxy();
    //@}

    //! @name Operations
    //@{
    void SwitchToFixed();
    void SwitchToCameraFixed();
    void SwitchToSimulationTime();
    virtual void Set();
    //@}

    //! @name Fixed lighting
    //@{
    void SetLightDirection( const geometry::Vector3f& direction );
    void SetAmbient       ( float r, float g, float b );
    void SetDiffuse       ( float r, float g, float b );
    //@}

    //! @name Time lighting
    //@{
    void SetLatitude( float latitude );
    void SetTime    ( const QDateTime& time );
    //@}

private:
    //! @name Member data
    //@{
    bool camera_;
    std::unique_ptr< FixedLighting > fixed_;
    std::unique_ptr< TimeLighting > time_;
    Lighting_ABC* current_;
    //@}
};

}

#endif // __LightingProxy_h_
