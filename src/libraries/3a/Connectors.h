// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Connectors_h_
#define __Connectors_h_

#include "Functions.h"
#include "HandlerToFunction.h"
#include "Slots.h"
#include "ValueHandler_ABC.h"

#pragma warning (push)
#pragma warning (disable : 4355)

// =============================================================================
/** @class  Connector_ABC
    @brief  Connector_ABC
*/
// Created: AGE 2007-09-10
// =============================================================================
class Connector_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Connector_ABC() {};
    virtual ~Connector_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Connect( Slot_ABC& slot ) = 0;
    //@}
};

// =============================================================================
/** @class  FunctionConnector
    @brief  FunctionConnector
*/
// Created: AGE 2007-09-10
// =============================================================================
template< typename K, typename A >
class FunctionConnector : public Function1_ABC< K, A >
                        , public Connector_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FunctionConnector() : handlers_( *this ), next_( 0 ) {};
    virtual ~FunctionConnector() {};
    //@}

    //! @name Operations
    //@{
    virtual void Connect( Slot_ABC& slot )
    {
        next_ = & slot.Cast< Function1_ABC< K, A > >();
    };
    //@}

    //! @name Member data
    //@{
    HandlerToFunction< K, A > handlers_;
    //@}

private:
    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        if( next_ ) next_->BeginTick();
    }
    virtual void SetKey( const K& key )
    {
        if( next_ ) next_->SetKey( key );
    }
    virtual void Apply( const A& arg )
    {
        if( next_ ) next_->Apply( arg );
    }
    virtual void EndTick()
    {
        if( next_ ) next_->EndTick();
    }
    //@}

    //! @name Member data
    //@{
    Function1_ABC< K, A >* next_;
    //@}
};

// =============================================================================
/** @class  HandlerConnector
    @brief  HandlerConnector
*/
// Created: AGE 2007-09-10
// =============================================================================
template< typename T >
class HandlerConnector : public ValueHandler_ABC< T >
                       , public Connector_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HandlerConnector() : next_( 0 ) {};
    virtual ~HandlerConnector() {};
    //@}

    //! @name Operations
    //@{
    virtual void Connect( Slot_ABC& slot )
    {
        next_ = & slot.Cast< ValueHandler_ABC< T > >();
    };
    //@}

private:
    //! @name Operations
    //@{
    virtual void Handle( const T& value )
    {
        if( next_ ) next_->Handle(  value );
    }
    //@}

private:
    //! @name Member data
    //@{
    ValueHandler_ABC< T >* next_;
    //@}
};

#pragma warning (pop)

#endif // __Connectors_h_
