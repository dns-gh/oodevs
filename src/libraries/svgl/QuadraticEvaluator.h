// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __QuadraticEvaluator_h_
#define __QuadraticEvaluator_h_

class Tesselator_ABC;

// =============================================================================
/** @class  QuadraticEvaluator
    @brief  QuadraticEvaluator
*/
// Created: AGE 2006-08-12
// =============================================================================
class QuadraticEvaluator
{

public:
    //! @name Constructors/Destructor
    //@{
             QuadraticEvaluator( const geometry::Point& from, const geometry::Point& c, const geometry::Point& to );
    virtual ~QuadraticEvaluator();
    //@}

    //! @name Operations
    //@{
    void Evaluate( Tesselator_ABC& tesselator, unsigned n ) const;
    //@}

private:
    //! @name Helpers
    //@{
    geometry::Point Evaluate( float u ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point from_;
    geometry::Point control_;
    geometry::Point to_;
    //@}
};

#endif // __QuadraticEvaluator_h_
