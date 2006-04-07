// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionVariant_h_
#define __OptionVariant_h_

#include "boost/any.hpp"

// =============================================================================
/** @class  OptionVariant
    @brief  Option variant. Just a useless wrapper.
            // $$$$ AGE 2006-02-13: add a save ? or a ToString ? or use a boost::variant ?
*/
// Created: AGE 2006-02-13
// =============================================================================
class OptionVariant
{

public:
    //! @name Constructors/Destructor
    //@{
                 OptionVariant() {};
                 template< typename T >
    /*implicit*/ OptionVariant( const T& value ) : value_( value ) {};
    //@}

    //! @name Operations
    //@{
    template< typename T >
    T To() const
    {
        try
        {
            return boost::any_cast< T >( value_ );
        } catch( ... )
        {
            throw std::runtime_error( std::string( "OptionVariant : Could not cast from " ) + value_.type().name() + " to " + typeid( T ).name() );
        }
    };
    //@}

private:
    //! @name Member data
    //@{
    boost::any value_;
    //@}
};

#endif // __OptionVariant_h_
