// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GQ_PlotDataBBox_h_
#define __GQ_PlotDataBBox_h_

#include <vector>

namespace gui
{

// =============================================================================
/** @class  GQ_PlotDataBBox
    @brief  GQ_PlotDataBBox
    @par    Using example
    @code
    GQ_PlotDataBBox;
    @endcode
*/
// Created: CBX 2003-08-08
// =============================================================================
class GQ_PlotDataBBox
{
public:
    typedef std::pair< double, double > T_Point;
    typedef std::vector< T_Point >      T_Data;

public:
    //! @name Constructors/Destructor
    //@{
    GQ_PlotDataBBox();

    GQ_PlotDataBBox& operator= ( const GQ_PlotDataBBox& );
    bool             operator==( const GQ_PlotDataBBox& ) const;
    //@}

    //! @name Operations
    //@{
    bool Reset();
    //@}

    //! @name Accessors
    //@{
    bool   IsEmpty() const;

    double XMin() const;
    double XMax() const;

    double YMin() const;
    double YMax() const;

    T_Point Center() const;
    //@}

    //! @name Inclusion tests
    //@{
    bool Includes  ( const T_Point&         ) const;
    bool Includes  ( const GQ_PlotDataBBox& ) const;
    bool Intersects( const GQ_PlotDataBBox& ) const;

    double GetDistanceTo( const T_Point& ) const;
    //@}

    //! @name Modifiers
    //@{
    bool SetBBox ( const GQ_PlotDataBBox& );
    bool UpdateWithPoint( const T_Point& );
    bool UpdateWithBBox ( const GQ_PlotDataBBox& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GQ_PlotDataBBox( const GQ_PlotDataBBox& rhs );            //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    void AddPoint( const T_Point& );
    //@}

private:
    //! @name Member data
    //@{
    bool   bEmpty_;
    double rXMin_;
    double rXMax_;
    double rYMin_;
    double rYMax_;
    //@}
};

// =============================================================================
// Inline methods implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::IsEmpty
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
bool GQ_PlotDataBBox::IsEmpty() const
{
    return bEmpty_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::XMin
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
double GQ_PlotDataBBox::XMin() const
{
    return rXMin_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::XMax
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
double GQ_PlotDataBBox::XMax() const
{
    return rXMax_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::YMin
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
double GQ_PlotDataBBox::YMin() const
{
    return rYMin_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::YMax
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
double GQ_PlotDataBBox::YMax() const
{
    return rYMax_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotDataBBox::Center
/** @return
*/
// Created: CBX 2004-03-08
// -----------------------------------------------------------------------------
inline
GQ_PlotDataBBox::T_Point GQ_PlotDataBBox::Center() const
{
    if( bEmpty_ )
        return T_Point( 0.0, 0.0 );

    return T_Point( 0.5 * ( rXMin_ + rXMax_ ), 0.5 * ( rYMin_ + rYMax_ ) );
}

}

#endif // __GQ_PlotDataBBox_h_
