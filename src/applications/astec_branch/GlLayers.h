// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlLayers_h_
#define __GlLayers_h_

#include "GlProxy.h"
#include "WorldParameters.h"

class Controllers;
class Model;
class ColorStrategy;
class ElevationMap;
class ParametersLayer;

// =============================================================================
/** @class  GlLayers
    @brief  GlLayers
*/
// Created: AGE 2006-03-29
// =============================================================================
class GlLayers : private WorldParameters, public GlProxy
{

public:
    //! @name Constructors/Destructor
    //@{
             GlLayers( const std::string& scipioXml, Controllers& controllers, Model& model );
    virtual ~GlLayers();
    //@}

    //! @name Accessors
    //@{
    // $$$$ AGE 2006-03-31: 
    ElevationMap&    GetElevationMap()    { return *elevation_; };
    ParametersLayer& GetParametersLayer() { return *parameters_; };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlLayers( const GlLayers& );            //!< Copy constructor
    GlLayers& operator=( const GlLayers& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ColorStrategy* strategy_;
    ElevationMap*  elevation_; // $$$$ AGE 2006-04-04: Move out !
    ParametersLayer* parameters_;

    // $$$$ AGE 2006-03-29: 
    geometry::Rectangle2f viewport_;
    unsigned int frame_;
    //@}
};

#endif // __GlLayers_h_
