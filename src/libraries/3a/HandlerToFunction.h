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

#pragma warning (push)
#pragma warning (disable : 4355)

// =============================================================================
/** @class  HandlerToFunction
    @brief  HandlerToFunction
*/
// Created: AGE 2007-08-29
// =============================================================================
template< typename K, typename A >
class HandlerToFunction : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit HandlerToFunction( Function1_ABC< K, A >& function )
        : function_( function )
        , key_     ()
        , value_   ( this )
    {
        // NOTHING
    }
    virtual ~HandlerToFunction()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    ValueHandler_ABC< K >& KeyParameter() { return key_; }
    ValueHandler_ABC< A >& Parameter()    { return value_; }
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    struct ValueParameter : public ValueHandler_ABC< T >
    {
        ValueParameter( HandlerToFunction* that )
            : that_ ( that )
            , value_()
        {
            // NOTHING
        }
        virtual void BeginTick()
        {
            that_->function_.BeginTick();
        }
        virtual void Handle( const T& value )
        {
            value_ = value;
            that_->Commit();
        }
        virtual void EndTick()
        {
            that_->function_.EndTick();
        }
        T value_;
        HandlerToFunction* that_;
    };
    template< typename T >
    struct ValueHolder : public ValueHandler_ABC< T >
    {
        ValueHolder()
            : value_()
            , set_  ( false )
        {
            // NOTHING
        }
        virtual void BeginTick() {}
        virtual void Handle( const T& value )
        {
            value_ = value;
            set_ = true;
        }
        virtual void EndTick() {}
        T value_;
        bool set_;
    };
    void Commit()
    {
        if( key_.set_ )
            function_.SetKey( key_.value_ );
        function_.Apply( value_.value_ );
        key_.set_ = false;
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, A >& function_;
    ValueHolder< K > key_;
    ValueParameter< A > value_;
    //@}
};

#pragma warning (pop)

#endif // __HandlerToFunction_h_
