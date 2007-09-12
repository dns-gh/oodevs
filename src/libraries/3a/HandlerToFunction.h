// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HandlerToFunction_h_
#define __HandlerToFunction_h_

#include "Functions.h"
#include "ValueHandler_ABC.h"
#include <boost/shared_ptr.hpp>

#pragma warning (push)
#pragma warning (disable : 4355)

// =============================================================================
/** @class  HandlerToFunction
    @brief  HandlerToFunction
*/
// Created: AGE 2007-08-29
// =============================================================================
template< typename K, typename A >
class HandlerToFunction
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit HandlerToFunction( Function1_ABC< K, A >& function )
        : function_( function ), key_( this ), value_( this ) {};
    //@}

    //! @name Operations
    //@{
    ValueHandler_ABC< K >& KeyParameter()  { return key_; }
    ValueHandler_ABC< A >& Parameter()     { return value_; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HandlerToFunction( const HandlerToFunction& );            //!< Copy constructor
    HandlerToFunction& operator=( const HandlerToFunction& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    struct ValueParameter : public ValueHandler_ABC< T >
    {
        ValueParameter( HandlerToFunction* that ) : that_( that ), value_(), set_( false ) {}
        virtual void Handle( const T& value ) {
            value_ = value;
            set_ = true;
            that_->Commit();
        }
        T value_;
        bool set_;
        HandlerToFunction* that_;
    };
    void Commit()
    {
        if( value_.set_ )
        {
            if( key_.set_ )
                function_.SetKey( key_.value_ );
            function_.Apply( value_.value_ );
            key_.set_ = value_.set_ = false;
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, A >& function_;
    ValueParameter< K > key_;
    ValueParameter< A > value_;
    //@}
};

#pragma warning (pop)

#endif // __HandlerToFunction_h_
