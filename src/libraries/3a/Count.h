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

#include "Functions.h"
#include "ValueHandler_ABC.h"
#include <set>

// =============================================================================
/** @class  Count
    @brief  Count
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename K, typename T >
class Count : public Function1_ABC< K, T >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Count( ValueHandler_ABC< unsigned >& handler )
        : handler_( handler ) {}
    virtual ~Count() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Count"; }
    virtual void BeginTick()
    {
        keys_.clear();
    }
    virtual void SetKey( const K& key )
    {
        keys_.insert( key );
    };
    virtual void Apply( const T& )
    {
        // NOTHING
    }
    virtual void EndTick()
    {
        handler_.Handle( keys_.size() );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Count( const Count& );            //!< Copy constructor
    Count& operator=( const Count& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ValueHandler_ABC< unsigned >& handler_;
    std::set< K > keys_;
    //@}
};

#endif // __Count_h_
