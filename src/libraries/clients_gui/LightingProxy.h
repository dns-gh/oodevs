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
             LightingProxy();
    explicit LightingProxy( const LightingProxy& other );
    virtual ~LightingProxy();
    //@}

    //! @name Operations
    //@{
    void SwitchToFixed();
    void SwitchToCameraFixed();
    void SwitchToSimulationTime();
    virtual void Set();
    virtual std::shared_ptr< Lighting_ABC > Clone() const;
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
    std::shared_ptr< FixedLighting > fixed_;
    std::shared_ptr< TimeLighting > time_;
    std::shared_ptr< Lighting_ABC > current_;
    //@}
};

}

#endif // __LightingProxy_h_
