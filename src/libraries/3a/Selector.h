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

#include "Reductor_ABC.h"

// =============================================================================
/** @class  Selector
    @brief  Selector
*/
// Created: AGE 2007-08-29
// =============================================================================
template< typename K, typename T >
class Selector : public Reductor_ABC< K, T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Selector( const K& key, Function1_ABC< K, T >& handler )
                : key_( key ), handler_( handler ), found_( false ) {}
    virtual ~Selector() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Selector"; }
    virtual void OnBeginTick()
    {
        found_ = false;
        handler_.BeginTick();
    }
    virtual void SetKey( const K& key )
    {
        found_ = key_ == key;
    }
    virtual void Apply( const T& value )
    {
        if( found_ )
            handler_.Apply( value );
    }
    virtual void OnEndTick()
    {
        handler_.EndTick();
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
    Function1_ABC< K, T >& handler_;
    bool found_;
    //@}
};

#endif // __Selector_h_
