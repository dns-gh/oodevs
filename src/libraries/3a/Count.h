// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Count_h_
#define __Count_h_

#include "Reductor_ABC.h"
#include <set>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Count
    @brief  Count
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename K, typename T >
class Count : public Reductor_ABC< K, T >
{
public:
    //! @name Types
    //@{
    typedef NumericValue Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Count( xml::xistream&, Function1_ABC< K, NumericValue >& handler )
        : handler_( handler )
        , set_    ( false )
    {
        // NOTHING
    }
    explicit Count( Function1_ABC< K, NumericValue >& handler )
        : handler_( handler )
        , set_    ( false )
    {
        // NOTHING
    }
    virtual ~Count()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Count"; }
    virtual void OnBeginTick()
    {
        handler_.BeginTick();
        keys_.clear();
    }
    virtual void SetKey( const K& key )
    {
        set_ = true;
        current_ = key;
    };
    virtual void Apply( const T& )
    {
        if( set_ )
            keys_.insert( current_ );
        set_ = false;
    }
    virtual void OnEndTick()
    {
        handler_.Apply( static_cast< unsigned int >( keys_.size() ) );
        handler_.EndTick();
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, NumericValue >& handler_;
    std::set< K > keys_;
    K current_;
    bool set_;
    //@}
};

#endif // __Count_h_
