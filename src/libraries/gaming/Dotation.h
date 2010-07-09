// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Dotation_h_
#define __Dotation_h_

namespace kernel
{
    class DotationType;
}

// =============================================================================
/** @class  Dotation
    @brief  Dotation
*/
// Created: AGE 2006-02-21
// =============================================================================
class Dotation
{
public:
    //! @name Constructors/Destructor
    //@{
             Dotation();
    explicit Dotation( const kernel::DotationType& type, unsigned int quantity = 0 );
    virtual ~Dotation();
    //@}

    //! @name Operators
    //@{
    Dotation operator+( const Dotation& rhs ) const;
    Dotation operator-( const Dotation& rhs ) const;
    Dotation operator-() const;
    //@}

public:
    //! @name Member data
    //@{
    const kernel::DotationType* type_;
    int quantity_;
    //@}
};

#endif // __Dotation_h_
