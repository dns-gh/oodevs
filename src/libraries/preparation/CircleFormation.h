// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __CircleFormation_h_
#define __CircleFormation_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  CircleFormation
    @brief  CircleFormation
*/
// Created: JSR 2012-07-02
// =============================================================================
class CircleFormation : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             CircleFormation( const geometry::Point2f& center, unsigned int numberOfAgents, float radius = 100.f);
    virtual ~CircleFormation();
    //@}

    //! @name Operations
    //@{
    geometry::Point2f NextPosition( bool isPc );
    //@}

private:
    //! @name Member data
    //@{
    geometry::Vector2f center_;
    float radius_;
    unsigned int current_;
    float angle_;
    //@}
};

#endif // __CircleFormation_h_
