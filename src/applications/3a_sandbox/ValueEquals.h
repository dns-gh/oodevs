// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueEquals_h_
#define __ValueEquals_h_

#include "ValueHolder_ABC.h"

// =============================================================================
/** @class  ValueEquals
    @brief  ValueEquals
*/
// Created: AGE 2007-07-12
// =============================================================================
class ValueEquals : public ValueHolder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ValueEquals( ValueHolder_ABC& holder, float expected )
                 : holder_( holder )
                 , expected_( expected ) {}
    //@}

    //! @name Operations
    //@{
    virtual float GetValue() const
    {
        return holder_.GetValue() == expected_ ? 1.f : 0.f;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ValueEquals( const ValueEquals& );            //!< Copy constructor
    ValueEquals& operator=( const ValueEquals& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ValueHolder_ABC& holder_;
    float expected_;
    //@}
};

#endif // __ValueEquals_h_
