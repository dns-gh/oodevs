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
#include "ValueHandler_ABC.h"
#include "KeyMarshaller.h"

#pragma warning (push)
#pragma warning (disable : 4355 )
#include <vector>
#include <boost/bind.hpp>

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
             Connector_ABC() {}
    virtual ~Connector_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Connect( Slot_ABC& slot, unsigned int i ) = 0;
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
    FunctionConnector()
        : handlers_( *this )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void Connect( Slot_ABC& slot, unsigned int i )
    {
        next_.push_back( & slot.Cast< Function1_ABC< K, A > >( i ) );
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
        for( std::vector< Function1_ABC< K, A >* >::const_iterator it = next_.begin(); it != next_.end(); ++it )
            (*it)->BeginTick();
    }
    virtual void SetKey( const K& key )
    {
        for( std::vector< Function1_ABC< K, A >* >::const_iterator it = next_.begin(); it != next_.end(); ++it )
            (*it)->SetKey( key );
    }
    virtual void Apply( const A& arg )
    {
        for( std::vector< Function1_ABC< K, A >* >::const_iterator it = next_.begin(); it != next_.end(); ++it )
            (*it)->Apply( arg );
    }
    virtual void EndTick()
    {
        for( std::vector< Function1_ABC< K, A >* >::const_iterator it = next_.begin(); it!= next_.end(); ++it )
            (*it)->EndTick();
    }
    //@}

    //! @name Member data
    //@{
    std::vector< Function1_ABC< K, A >* > next_;
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
    HandlerConnector() {}
    //@}

    //! @name Operations
    //@{
    virtual void Connect( Slot_ABC& slot, unsigned i )
    {
        next_ = & slot.Cast< ValueHandler_ABC< T > >( i );
    };
    //@}

private:
    //! @name Operations
    //@{
    virtual void Handle( const T& value )
    {
        for( std::vector< ValueHandler_ABC< T >* >::const_iterator it = next_.begin(); it != next_.end(); ++it )
            (*it)->Handle( value );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::vector< ValueHandler_ABC< T >* > next_;
    //@}
};

#pragma warning (pop)

#endif // __Connectors_h_
