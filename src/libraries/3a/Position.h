// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "game_asn/Simulation.h"
#include <string>
#pragma warning (push)
#pragma warning (disable : 4702 )
#include <geometry/Types.h>
#pragma warning (pop)

// =============================================================================
/** @class  Position
    @brief  Position
*/
// Created: AGE 2007-09-12
// =============================================================================
class Position
{

public:
    //! @name Constructors/Destructor
    //@{     
             Position();
    explicit Position( const std::string& coord );
    explicit Position( const ASN1T_CoordLatLong& coord );
            ~Position();
    //@}

    //! @name Operations
    //@{
    float Distance( const Position& rhs ) const;
    bool operator<( const Position& rhs ) const;
    bool operator>( const Position& rhs ) const;
    bool operator==( const Position& rhs ) const;
    bool operator!=( const Position& rhs ) const;

    operator double() const;

    static geometry::Polygon2f ToPolygon( const std::vector< Position >& positions );
    bool IsInside( const geometry::Polygon2f& polygon ) const;
    //@}

    //! @name Compilation compatibility
    //@{
    explicit Position( unsigned int ) {};

    template< typename T >
    Position& operator+=( const T& ) { return *this; }
    template< typename T >
    Position operator+( const T& ) const { return *this; }
    template< typename T >
    Position operator-( const T& ) const { return *this; }
    template< typename T >
    Position operator/( const T& ) const { return *this; }
    //@}

    //! @name Operators
    //@{
    friend std::ostream& operator<<( std::ostream& os, const Position& position );
    friend std::istream& operator>>( std::istream& is, Position& position );
    //@}

private:
    //! @name Helpers
    //@{
    const geometry::Point2f& Point() const;
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_CoordLatLong coord_;
    mutable bool init_;
    mutable geometry::Point2f point_;
    //@}
};

#endif // __Position_h_
