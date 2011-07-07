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

namespace kernel
{
    class Settings_ABC;

    namespace detail
    {
        class ValueHolder_ABC;
    }

// =============================================================================
/** @class  OptionVariant
    @brief  Option variant.
*/
// Created: AGE 2006-02-13
// =============================================================================
class OptionVariant
{
public:
    //! @name Constructors/Destructor
    //@{
                 OptionVariant();
                 template< typename T >
    /*implicit*/ OptionVariant( const T& value );
                 template< typename T >
                 OptionVariant( Settings_ABC& settings, const std::string& name, const T& defaultValue );
                 OptionVariant( const OptionVariant& rhs );
                ~OptionVariant();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    T To() const;

    void Save( Settings_ABC& settings, const std::string& name ) const;
    OptionVariant& operator=( const OptionVariant& rhs );
    //@}

private:
    //! @name Member data
    //@{
    detail::ValueHolder_ABC* holder_;
    //@}
};

}

#include "Settings_ABC.h"

namespace kernel
{

namespace detail
{
    class ValueHolder_ABC
    {
    public:
        virtual ~ValueHolder_ABC() {};
        virtual std::string Typename() const = 0;
        virtual void Save( Settings_ABC& settings, const std::string& name ) const = 0;
        virtual void Load( Settings_ABC& settings, const std::string& name ) = 0;
        virtual ValueHolder_ABC* Clone() const = 0;
    };

    template< typename T >
    class ValueHolder : public ValueHolder_ABC
    {
    public:
        /*implicit*/ ValueHolder( const T& value ) : value_( value ) {};
        virtual ~ValueHolder() {};
        virtual std::string Typename() const
        {
            return typeid( T ).name();
        }
        virtual void Save( Settings_ABC& settings, const std::string& name ) const
        {
            settings.Save( name, value_ );
        }
        virtual void Load( Settings_ABC& settings, const std::string& name )
        {
            value_ = settings.Load( name, value_ );
        }
        virtual ValueHolder_ABC* Clone() const
        {
            return new ValueHolder( value_ );
        }

        T value_;
    };
}

// -----------------------------------------------------------------------------
// Name: OptionVariant constructor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
template< typename T >
OptionVariant::OptionVariant( const T& value )
    : holder_( new detail::ValueHolder< T >( value ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OptionVariant::To
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
template< typename T >
T OptionVariant::To() const
{
    if( ! dynamic_cast< detail::ValueHolder< T >* >( holder_ ) )
        throw std::runtime_error( std::string( "OptionVariant : Could not cast from " ) + holder_->Typename() + " to " + typeid( T ).name() );
    return static_cast< detail::ValueHolder< T >* >( holder_ )->value_;
}

// -----------------------------------------------------------------------------
// Name: OptionVariant constructor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
template< typename T >
OptionVariant::OptionVariant( Settings_ABC& settings, const std::string& name, const T& defaultValue )
    : holder_( new detail::ValueHolder< T >( defaultValue ) )
{
    holder_->Load( settings, name );
}

}

#endif // __OptionVariant_h_
