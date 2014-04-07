// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionalValue_h_
#define __OptionalValue_h_

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  OptionalValue
    @brief  Optional value
*/
// Created: AGE 2006-02-09
// =============================================================================
template< typename T >
class OptionalValue
{
public:
    //! @name Constructors/Destructor
    //@{
                 OptionalValue() : value_(), set_( false ) {}
    /*implicit*/ OptionalValue( const T& value, bool isSet = true ) : value_( value ), set_( isSet ) {}
    virtual     ~OptionalValue() {}
    //@}

    //! @name Operations
    //@{
    T&   Data() { return value_; }
    const T& Data() const { return value_; }
    void Set() { set_ = true; }
    bool IsSet() const { return set_; }
    //@}

    //! @name Operators
    //@{
    T operator->() { return value_; }
    operator const T&() const { return value_; }
    OptionalValue& operator=( const T& value ) { value_ = value; set_ = true; return *this; }
    bool operator==( const T& value ) const { return value_ == value; }
    //@}

private:
    //! @name Member data
    //@{
    T value_;
    bool set_;
    //@}
};

template< typename T >
void ReaderHelper( OptionalValue< T >& value, const std::string& name, xml::xistream& xis )
{
    if( xis.has_attribute( name ) )
        value = xis.attribute< T >( name );
}

}

#endif // __OptionalValue_h_
