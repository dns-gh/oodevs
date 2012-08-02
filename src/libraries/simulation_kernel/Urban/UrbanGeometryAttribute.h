// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanGeometryAttribute_h_
#define __UrbanGeometryAttribute_h_

// TEMPORAIRE ?

#include "UrbanExtension_ABC.h"
#include <geometry/Types.h>

// =============================================================================
/** @class  UrbanGeometryAttribute
    @brief  UrbanGeometryAttribute
*/
// Created: JSR 2012-08-01
// =============================================================================
class UrbanGeometryAttribute : public UrbanExtension_ABC
                             , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanGeometryAttribute( xml::xistream& xis );
    virtual ~UrbanGeometryAttribute();
    //@}

public:
    //! @name Operations
    //@{
    // TODO passer tout en double (modifs sur spatial container?)
    const std::vector< geometry::Point2f >& Vertices() const;
    void SetGeometry( const std::vector< geometry::Point2f >& vertices );
    float ComputeArea() const;
    geometry::Rectangle2f BoundingBox() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< geometry::Point2f > vertices_;
    //@}
};

#endif // __UrbanGeometryAttribute_h_
