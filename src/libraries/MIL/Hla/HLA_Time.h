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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Time.h $
// $Author: Age $
// $Modtime: 5/11/04 11:03 $
// $Revision: 1 $
// $Workfile: HLA_Time.h $
//
// *****************************************************************************

#ifndef __Time_h_
#define __Time_h_

#include "hla/Time_ABC.h"

// =============================================================================
/** @class  HLA_Time
    @brief  Time implementation
*/
// Created: AGE 2004-10-13
// =============================================================================
class HLA_Time : public Time_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Time();
    explicit HLA_Time( double rTime );
    virtual ~HLA_Time();
    //@}

    //! @name Operations
    //@{
    virtual HLA_Time& Clone() const;

    virtual HLA_Time& SetInitial();
    virtual bool IsInitial() const;
    virtual HLA_Time& SetFinal();
    virtual bool IsFinal() const;

    virtual HLA_Time& Set( Time_ABC const & rhs );
    virtual HLA_Time& IncreaseBy( TimeInterval_ABC const & interval );
    virtual HLA_Time& DecreaseBy( TimeInterval_ABC const & interval );
    virtual TimeInterval_ABC& Substract( Time_ABC const & rhs ) const; // Create !

    virtual bool IsGreaterThan( Time_ABC const & rhs ) const;
    virtual bool IsLessThan( Time_ABC const & rhs ) const;
    virtual bool IsEqualTo( Time_ABC const & rhs ) const;
    virtual bool IsGreaterThanOrEqualTo( Time_ABC const & rhs ) const;
    virtual bool IsLessThanOrEqualTo( Time_ABC const & rhs ) const;

    virtual void Encode( Serializer& serializer ) const;
    virtual std::string ToString() const;
    //@}

private:
    //! @name Member data
    //@{
    double rCurrentTime_;
    //@}
};

#endif // __Time_h_
