// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CubicEvaluator_h_
#define __CubicEvaluator_h_

class Tesselator_ABC;

// =============================================================================
/** @class  CubicEvaluator
    @brief  CubicEvaluator
*/
// Created: AGE 2006-08-12
// =============================================================================
class CubicEvaluator
{

public:
    //! @name Constructors/Destructor
    //@{
             CubicEvaluator( const geometry::Point& from, const geometry::Point& c1, const geometry::Point& c2, const geometry::Point& to );
    virtual ~CubicEvaluator();
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
    geometry::Point c1_;
    geometry::Point c2_;
    geometry::Point to_;
    //@}
};

#endif // __CubicEvaluator_h_
