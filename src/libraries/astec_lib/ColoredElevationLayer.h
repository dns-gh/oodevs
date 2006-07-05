// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColoredElevationLayer_h_
#define __ColoredElevationLayer_h_

#include "graphics/ElevationLayer.h"
namespace xml { class xistream; }
class DetectionMap;

// =============================================================================
/** @class  ColoredElevationLayer
    @brief  Colored elevation layer
*/
// Created: AGE 2006-04-19
// =============================================================================
class ColoredElevationLayer : public ElevationLayer
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ColoredElevationLayer( const DetectionMap& elevation );
    virtual ~ColoredElevationLayer();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColoredElevationLayer( const ColoredElevationLayer& );            //!< Copy constructor
    ColoredElevationLayer& operator=( const ColoredElevationLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void SelectColor( short elevation, float slope, short maxElevation, unsigned char* color );
    void WriteColor( const QColor& color, unsigned char* where );
    void ReadElevation( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::pair< float, QColor > T_Color;
    typedef std::vector< T_Color >     T_Colors;
    typedef T_Colors::const_iterator CIT_Colors;
    //@}

private:
    //! @name Member data
    //@{
    T_Colors colors_;
    //@}
};

#endif // __ColoredElevationLayer_h_
