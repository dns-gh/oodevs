// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef TerrainSettings_h
#define TerrainSettings_h

#include <graphics/GraphicSetup_ABC.h>
#include <spatialcontainer/TerrainData.h>

class Data_ABC;

namespace kernel
{
    class Options;
}

namespace gui
{
    struct TerrainSetting;

// =============================================================================
/** @class  TerrainSettings
    @brief  TerrainSettings
*/
// Created: ABR 2014-07-07
// =============================================================================
class TerrainSettings : public GraphicSetup_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainSettings();
    virtual ~TerrainSettings();
    //@}

    //! @name Operations
    //@{
    void Load( const kernel::Options& options );
    void SetAlpha( float alpha );
    void Apply( const std::function< void( const std::shared_ptr< TerrainSetting >& ) >& functor ) const;
    //@}

    //! @name GraphicSetup_ABC implementation
    //@{
    virtual void SetupLineGraphics( const Data_ABC* pData );
    virtual void SetupLineGraphics( unsigned int offset );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics( const Data_ABC* pData );
    //@}

private:
    //! @name Member data
    //@{
    std::map< TerrainData, std::shared_ptr< TerrainSetting > > settings_;
    float alpha_;
    //@}
};

} //! namespace gui

#endif // TerrainSettings_h
