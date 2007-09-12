// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Selector_h_
#define __Selector_h_

#include "Functions.h"
#include "ValueHandler_ABC.h"

// =============================================================================
/** @class  Selector
    @brief  Selector
*/
// Created: AGE 2007-08-29
// =============================================================================
template< typename K, typename T >
class Selector : public Function1_ABC< K, T >
{

public:
    //! @name Constructors/Destructor
    //@{
             Selector( const K& key, ValueHandler_ABC< T >& handler )
                : key_( key ), handler_( handler ), found_( false ) {}
    virtual ~Selector() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Selector"; }
    virtual void BeginTick()
    {
        found_ = false;
    }
    virtual void SetKey( const K& key )
    {
        found_ = key_ == key;
    }
    virtual void Apply( const T& value )
    {
        if( found_ )
            handler_.Handle( value );
    }
    virtual void EndTick()
    {
        // NOTHING
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Selector( const Selector& );            //!< Copy constructor
    Selector& operator=( const Selector& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const K key_;
    ValueHandler_ABC< T >& handler_;
    bool found_;
    //@}
};

#endif // __Selector_h_
