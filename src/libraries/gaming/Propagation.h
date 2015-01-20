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

#include <geometry/Types.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class DisasterType;
}

class Extractor_ABC;
class RGBATextureFactory;
class TextureTree;

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
             Propagation( const Extractor_ABC& extractor,
                          const kernel::CoordinateConverter_ABC& converter,
                          const kernel::DisasterType& disasterType );
    virtual ~Propagation();
    //@}

    //! @name Operations
    //@{
    void Draw() const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< RGBATextureFactory > pFactory_;
    std::unique_ptr< TextureTree > pTree_;
    geometry::Rectangle2f globalExtent_;
    //@}
};

#endif // Propagation_h
