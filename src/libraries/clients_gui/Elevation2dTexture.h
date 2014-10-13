// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef Elevation2dTexture_h
#define Elevation2dTexture_h

#include <boost/noncopyable.hpp>

namespace kernel
{
    class DetectionMap;
    class Options;
}

class ElevationShader;

namespace gui
{
    class ElevationExtrema;
    class Gradient;
    class GradientPreferences;

// =============================================================================
/** @class  Elevation2dTexture
    @brief  Elevation2dTexture
*/
// Created: ABR 2014-07-03
// =============================================================================
class Elevation2dTexture : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Elevation2dTexture( const kernel::DetectionMap& map );
    virtual ~Elevation2dTexture();
    //@}

    //! @name Operations
    //@{
    void Load( const kernel::Options& options );
    void Purge();

    void ConfigureShader( ElevationShader& shader );
    unsigned int CreateTexture();

    const geometry::Rectangle2f& GetViewport() const;
    void SetViewport( const geometry::Rectangle2f& viewport,
                      const ElevationExtrema& extrema );

    std::shared_ptr< GradientPreferences > GetPreferences();
    std::shared_ptr< Gradient > GetGradient() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::shared_ptr< kernel::Options > options_;
    const kernel::DetectionMap& map_;
    unsigned int texture_;

    std::shared_ptr< GradientPreferences > gradientPreferences_;
    geometry::Rectangle2f viewport_;
    bool hsEnabled_;
    int hsAngle_;
    float hsx_;
    float hsy_;
    float hsStrength_;
    short minElevation_;
    short maxElevation_;
    float alpha_;
    //@}
};

} //! namespace gui

#endif // Elevation2dTexture_h
