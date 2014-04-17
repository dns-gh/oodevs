// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef Extractor_ABC_h
#define Extractor_ABC_h

#include <boost/noncopyable.hpp>
#include <geometry/Types.h>
#include <vector>

// =============================================================================
/** @class  Extractor_ABC
    @brief  Extractor_ABC
*/
// Created: JSR 2014-04-14
// =============================================================================
class Extractor_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< float > T_Values;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Extractor_ABC();
    virtual ~Extractor_ABC();
    //@}

    //! @name Accessors
    //@{
    int GetCols() const;
    int GetRows() const;
    const geometry::Rectangle2d& GetExtent() const;
    const T_Values& GetValues() const;
    const geometry::Point2d& GetPixelSize() const;
    float GetValue( double longitude, double latitude ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Fill( std::vector< geometry::Point2d >& points ) const = 0;
    //@}

protected:
    //! @name Member data
    //@{
    int ncols_;
    int nrows_;
    geometry::Rectangle2d extent_;
    geometry::Point2d pixelSize_;
    T_Values values_;
    //@}
};

#endif // Extractor_ABC_h
