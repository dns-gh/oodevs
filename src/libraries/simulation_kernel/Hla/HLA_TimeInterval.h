// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-13 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TimeInterval.h $
// $Author: Age $
// $Modtime: 17/11/04 11:29 $
// $Revision: 2 $
// $Workfile: HLA_TimeInterval.h $
//
// *****************************************************************************

#ifndef __TimeInterval_h_
#define __TimeInterval_h_

#include "hla/TimeInterval_ABC.h"

// =============================================================================
/** @class  HLA_TimeInterval
    @brief  Time interval implementation
*/
// Created: AGE 2004-10-13
// =============================================================================
class HLA_TimeInterval : public TimeInterval_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_TimeInterval();
             HLA_TimeInterval( double rInterval );
    virtual ~HLA_TimeInterval();
    //@}

    //! @name Operations
    //@{
    virtual HLA_TimeInterval& Clone() const;

    virtual bool IsZero() const;
    virtual HLA_TimeInterval& SetZero();

    virtual bool IsEpsilon() const;
    virtual HLA_TimeInterval& SetEpsilon();

    virtual HLA_TimeInterval& Set( TimeInterval_ABC const & rhs );
    virtual HLA_TimeInterval& IncreaseBy( TimeInterval_ABC const & rhs );
    virtual HLA_TimeInterval& DecreaseBy( TimeInterval_ABC const & rhs );

    virtual bool IsGreaterThan( TimeInterval_ABC const & rhs ) const;
    virtual bool IsLessThan( TimeInterval_ABC const & rhs ) const;
    virtual bool IsEqualTo( TimeInterval_ABC const & rhs ) const;
    virtual bool IsGreaterThanOrEqualTo( TimeInterval_ABC const & rhs ) const;
    virtual bool IsLessThanOrEqualTo( TimeInterval_ABC const & rhs ) const;

    virtual void Encode( Serializer& serializer ) const;
    virtual std::string ToString() const;
    //@}

    //! @name Accessors
    //@{
    double GetValue() const;
    //@}

private:
    //! @name Member data
    //@{
    static const double rEpsilon_;
    double rInterval_;
    //@}
};

#endif // __TimeInterval_h_
