// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VisionMap_h_
#define __VisionMap_h_

#include "Types.h"

namespace kernel
{
    class GlTools_ABC;
    class Viewport_ABC;
    class DetectionMap;
}

// =============================================================================
/** @class  VisionMap
    @brief  VisionMap
*/
// Created: AGE 2006-04-04
// =============================================================================
class VisionMap
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit VisionMap( const kernel::DetectionMap& map );
    virtual ~VisionMap();
    //@}

    //! @name Operations
    //@{
    void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void Incorporate( const geometry::Rectangle2< int >& extent );
    void Initialize();
    bool ShouldUpdate( const std::pair< unsigned, unsigned >& cell );
    void Update( const std::pair< unsigned, unsigned >& cell, kernel::E_PerceptionResult perception );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    VisionMap( const VisionMap& );            //!< Copy constructor
    VisionMap& operator=( const VisionMap& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    char DataAt( const std::pair< unsigned, unsigned >& cell );
    void Incorporate( const geometry::Point2< int >& point );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& map_;
    geometry::Rectangle2f boundingBox_;
    int left_, bottom_, right_, top_;
    int width_, height_;
    char* vision_;
    //@}
};

#endif // __VisionMap_h_
