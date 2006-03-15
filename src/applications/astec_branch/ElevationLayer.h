// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElevationLayer_h_
#define __ElevationLayer_h_

#include "graphics/MapLayer_ABC.h"
#include "graphics/DrawDetection.h"
#include <memory>

class DrawDetection;

// =============================================================================
/** @class  ElevationLayer
    @brief  ElevationLayer
*/
// Created: AGE 2006-03-15
// =============================================================================
class ElevationLayer : public MapLayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ElevationLayer( const std::string& detectionFile );
    virtual ~ElevationLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const geometry::Rectangle2f& viewport ); 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ElevationLayer( const ElevationLayer& );            //!< Copy constructor
    ElevationLayer& operator=( const ElevationLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string detectionFile_;
    std::auto_ptr< DrawDetection > detection_;
    //@}
};

#endif // __ElevationLayer_h_
