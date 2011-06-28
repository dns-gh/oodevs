// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __aar_Filter_h_
#define __aar_Filter_h_

#include "Functions.h"

namespace xml
{
    class xistream;
}

namespace aar
{

// =============================================================================
/** @class  Filter
    @brief  Filter
*/
// Created: AGE 2007-09-13
// =============================================================================
template< typename K, typename T >
class Filter : public Function2_ABC< K, bool, T >
{
public:
    //! @name
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Filter( xml::xistream&, Function1_ABC< K, Result_Type >& next )
        : next_( next )
    {
        // NOTHING
    }
    virtual ~Filter()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        next_.BeginTick();
    };
    virtual void SetKey( const K& key )
    {
        next_.SetKey( key );
    };
    virtual void Apply( const bool& test, const T& value )
    {
        if( test )
            next_.Apply( value );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    };
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string GetName() const { return "Filter"; }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, Result_Type >& next_;
    //@}
};

}

#endif // __aar_Filter_h_
