// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElevationExtrema_h_
#define __ElevationExtrema_h_

#include "pathfind/SpatialContainer.h"

class ElevationMap;

namespace gui
{

// =============================================================================
/** @class  ElevationExtrema
    @brief  ElevationExtrema
*/
// Created: AGE 2007-01-17
// =============================================================================
class ElevationExtrema
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ElevationExtrema( const ElevationMap& map );
    virtual ~ElevationExtrema();
    //@}

    //! @name Operations
    //@{
    void FindExtrema( const geometry::Rectangle2f& extent, short& min, short& max ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ElevationExtrema( const ElevationExtrema& );            //!< Copy constructor
    ElevationExtrema& operator=( const ElevationExtrema& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct Extrema
    {
        Extrema( int x, int y, int w, int h, short min, short max );
        geometry::Rectangle2< int > extent_;
        short min_;
        short max_;
        bool operator==( const Extrema& rhs ) const;
        int CompareOnX( int value ) const;
        int CompareOnY( int value ) const;
    };
    struct ExtremaFinder;
    typedef pathfind::SpatialContainer< Extrema, pathfind::DefaultTraits< Extrema, int >, int > T_Container;
    //@}

    //! @name Helpers
    //@{
    void Fill();
    void AddExtrema( int x, int y, int width, int height );
    //@}

private:
    //! @name Member data
    //@{
    const ElevationMap& map_;
    T_Container extrema_;
    //@}
};

}

#endif // __ElevationExtrema_h_
