// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RoadFormation_h_
#define __RoadFormation_h_

#include "Formation_ABC.h"
#include <vector>

namespace tic
{
// =============================================================================
/** @class  RoadFormation
    @brief  Road formation
*/
// Created: AGE 2008-04-01
// =============================================================================
class RoadFormation : public Formation_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RoadFormation( const std::vector< geometry::Point2f >& path );
    virtual ~RoadFormation();
    //@}

    //! @name Operations
    //@{
    virtual void Start( const geometry::Point2f& at, const geometry::Vector2f& towards, std::size_t count );
    virtual void Apply( Movable_ABC* movable );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RoadFormation( const RoadFormation& );            //!< Copy constructor
    RoadFormation& operator=( const RoadFormation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Fill( const geometry::Point2f& from, std::size_t count, float legnth );
    void AddPoint( const geometry::Point2f& from, const geometry::Point2f& to, float length, float segmentLenght );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< geometry::Point2f >    T_Points;
    typedef T_Points::const_reverse_iterator CRIT_Points;
    //@}

private:
    //! @name Member data
    //@{
    const T_Points& path_;
    T_Points result_;
    //@}
};
}

#endif // __RoadFormation_h_
