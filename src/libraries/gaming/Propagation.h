// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef Propagation_h
#define Propagation_h

#include <boost/noncopyable.hpp>
#include <graphics/ElevationMap.h>
#include <graphics/ElevationLayer.h>
#include <graphics/ElevationFactory.h>
#include <graphics/TextureTree.h>
#include <graphics/Visitor2d.h>
#include <graphics/TextureVisitor_ABC.h>

namespace
{
    class ColorFactory;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class ASCExtractor;
}

// =============================================================================
/** @class  Propagation
    @brief  Propagation
*/
// Created: LGY 2012-10-26
// =============================================================================
class Propagation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Propagation( const std::string& file, const std::string& projection,
                          const kernel::CoordinateConverter_ABC& converter );
    virtual ~Propagation();
    //@}

    //! @name Operations
    //@{
    void Draw() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< kernel::ASCExtractor > pExtractor_;
    std::auto_ptr< ElevationMap > pMap_;
    std::auto_ptr< ElevationFactory > pFactory_;
    std::auto_ptr< TextureTree > pTree_;
    std::auto_ptr< ColorFactory > pColor_;
    //@}
};

#endif // Propagation_h
