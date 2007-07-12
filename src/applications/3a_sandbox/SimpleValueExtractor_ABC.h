// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimpleValueExtractor_ABC_h_
#define __SimpleValueExtractor_ABC_h_

#include "ValueExtractor_ABC.h"
#include <limits>

// =============================================================================
/** @class  SimpleValueExtractor_ABC
    @brief  SimpleValueExtractor_ABC
*/
// Created: AGE 2007-07-12
// =============================================================================
class SimpleValueExtractor_ABC : public ValueExtractor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SimpleValueExtractor_ABC() : value_( 0 ) {};
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        value_ = ExtractValue( message );
    };
    virtual float GetValue() const
    {
        return value_;
    };
    //@}
    
protected:
    //! @name Operations
    //@{
    float Noop() const {
        return value_;
    }
    float Invalid() const {
        return std::numeric_limits< float >::infinity();
    }
    virtual float ExtractValue( const ASN1T_MsgsSimToClient& message ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleValueExtractor_ABC( const SimpleValueExtractor_ABC& );            //!< Copy constructor
    SimpleValueExtractor_ABC& operator=( const SimpleValueExtractor_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    float value_;
    //@}
};

#endif // __SimpleValueExtractor_ABC_h_
