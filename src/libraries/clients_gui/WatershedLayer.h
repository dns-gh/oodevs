// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __WatershedLayer_h_
#define __WatershedLayer_h_

#include "Layer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

class TextureSet;
class ElevationShader;

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{

// =============================================================================
/** @class  WatershedLayer
    @brief  WatershedLayer
*/
// Created: SBO 2010-03-23
// =============================================================================
class WatershedLayer : public Layer2d_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             WatershedLayer( kernel::Controllers& controllers, const kernel::DetectionMap& elevation );
    virtual ~WatershedLayer();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void Reset();
    //@}

    //! @name Accessors
    //@{
    void SetHeight( unsigned short height );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WatershedLayer( const WatershedLayer& );            //!< Copy constructor
    WatershedLayer& operator=( const WatershedLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateShader();
    void CreateTextures();
    void Cleanup();
    void SetGradient();
    void MakeGlTexture();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& elevation_;
    std::auto_ptr< ElevationShader > shader_;
    std::auto_ptr< TextureSet > layer_;
    bool reset_;
    bool modelLoaded_;
    bool ignore_;
    bool updateGradient_;
    unsigned gradientTexture_;
    const unsigned short gradientSize_;
    unsigned short height_;
    bool enabled_;
    bool inverted_;
    QColor color_;
    //@}
};

}

#endif // __WatershedLayer_h_
