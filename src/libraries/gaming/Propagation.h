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
#include <graphics/RGBATextureFactory.h>
#include <graphics/TextureTree.h>

namespace
{
    class ColorFactory;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class DisasterType;
}

class ASCExtractor;
class PropagationManager;

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
             Propagation( const std::string& file, const PropagationManager& manager,
                          const kernel::CoordinateConverter_ABC& converter, const kernel::DisasterType& disasterType );
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
    std::auto_ptr< ASCExtractor > pExtractor_;
    std::auto_ptr< RGBATextureFactory > pFactory_;
    std::auto_ptr< TextureTree > pTree_;
    //@}
};

#endif // Propagation_h
