// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Matrix_h_
#define __Matrix_h_

namespace geometry
{

// =============================================================================
/** @class  Matrix
    @brief  Matrix
*/
// Created: AGE 2006-08-17
// =============================================================================
class Matrix
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Matrix( float diag = 0 );
            ~Matrix();
    //@}

    //! @name Operations
    //@{
    Matrix& Multiply ( float a, float b, float c, float d, float e, float f );
    Matrix& Scale    ( float sx, float sy );
    Matrix& Translate( float tx, float ty );
    Matrix& Rotate   ( float angle );
    Matrix& SkewOnX  ( float angle );
    Matrix& SkewOnY  ( float angle );
    //@}

    //! @name Operators
    //@{
    operator const float*() const { return (const float*)( this ); };
    Matrix operator*( const Matrix& rhs ) const;
    Matrix& operator*=( const Matrix& rhs );
    //@}

private:
    //! @name Constructors
    //@{
    Matrix( float a, float b, float c, float d, float e, float f );
    //@}

    //! @name Helpers
    //@{
    static Matrix CreateScale( float sx, float sy );
    static Matrix CreateTranslate( float tx, float ty );
    static Matrix CreateRotation( float angle );
    static Matrix CreateSkewOnX( float angle );
    static Matrix CreateSkewOnY( float angle );
    //@}

private:
    //! @name Member data
    //@{
    float a00_, a10_, z1_, z6_;
    float a01_, a11_, z2_, z7_;
    float z3_ , z4_ , o1_, z5_;
    float a02_, a12_, z8_, o2_;
    //@}
};

}

#endif // __Matrix_h_
