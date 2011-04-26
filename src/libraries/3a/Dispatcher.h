// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dispatcher_h_
#define __Dispatcher_h_

#include "ModelFunction_ABC.h"
#include "ValueHandler_ABC.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <map>
#include <algorithm>
#pragma warning( pop )

// =============================================================================
/** @class  Dispatcher
    @brief  Dispatcher
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename KeyValue, typename FunctionFactory >
class Dispatcher : public ModelFunction_ABC
{
public:
    //! @name Types
    //@{
    typedef typename KeyValue::Type K;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Dispatcher( const FunctionFactory& factory = FunctionFactory(), const KeyValue& keyValue = KeyValue() )
                : factory_( factory )
                , keyValue_( keyValue )
                , frameWasEnded_( false )
    {
        // NOTHING
    }
    virtual ~Dispatcher()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        if( ! frameWasEnded_ )
            EndTick();
        frameWasEnded_ = false;
        for( IT_Functions it = dispatched_.begin(); it != dispatched_.end(); ++it )
            it->second->BeginTick();
    }
    virtual void Receive( const sword::SimToClient& message )
    {
        Handler handler( *this, message );
        EvaluateValue( keyValue_, message, handler );
    };
    virtual void EndTick()
    {
        frameWasEnded_ = true;
        for( IT_Functions it = dispatched_.begin(); it != dispatched_.end(); ++it )
            it->second->EndTick();
    }
    //@}

private:
    //! @name Helpers
    //@{
    struct Handler : public ValueHandler_ABC< K >
    {
        Handler( Dispatcher& that, const sword::SimToClient& message )
            : that_( &that ), message_( &message ) {}
        virtual void BeginTick() {}
        virtual void Handle( const K& value )
        {
            boost::shared_ptr< ModelFunction_ABC >& function = that_->dispatched_[ value ];
            if( ! function )
            {
                function = that_->factory_( value );
                // BeginTick should not be called if we are between two ticks, but only if a tick has already begun.
                if( !that_->frameWasEnded_ )
                    function->BeginTick();
            }
            function->Receive( *message_ );
        }
        virtual void EndTick() {}
        Dispatcher* that_;
        const sword::SimToClient* message_;
    };
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< K, boost::shared_ptr< ModelFunction_ABC > >  T_Functions;
    typedef typename T_Functions::iterator                        IT_Functions;
    //@}

private:
    //! @name Member data
    //@{
    FunctionFactory factory_;
    KeyValue        keyValue_;
    T_Functions     dispatched_;
    bool            frameWasEnded_;
    //@}
};

#endif // __Dispatcher_h_
