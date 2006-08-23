// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CoordinateConverter_ABC_h_
#define __CoordinateConverter_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  CoordinateConverter_ABC
    @brief  CoordinateConverter_ABC
*/
// Created: AGE 2006-05-17
// =============================================================================
class CoordinateConverter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CoordinateConverter_ABC() {};
    virtual ~CoordinateConverter_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool IsInBoundaries            ( const geometry::Point2f& point ) const = 0;
    virtual std::string       ConvertToMgrs( const geometry::Point2f& pos ) const = 0;
    virtual geometry::Point2f ConvertToGeo ( const geometry::Point2f& pos ) const = 0;

    template< typename T >
    geometry::Point2f ConvertToXY( const T& mgrs ) const
    {
        return DoConvertToXY( std::string( (const char*)( mgrs.data ), 15 ) );
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual geometry::Point2f DoConvertToXY( const std::string& mgrs ) const = 0;
    //@}
};

}

#endif // __CoordinateConverter_ABC_h_
