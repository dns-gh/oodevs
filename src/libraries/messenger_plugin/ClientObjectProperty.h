// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ClientObjectProperty_h_
#define __ClientObjectProperty_h_

#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.hpp>

namespace plugins
{
namespace messenger
{

class ClientObjectProperty_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientObjectProperty_ABC() {}
    virtual ~ClientObjectProperty_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( sword::ClientObjectProperty* msg ) const = 0;
    virtual void Write( xml::xostream& xos ) const = 0;
    //@}
};

// =============================================================================
/** @class  ClientObjectProperty
    @brief  ClientObjectProperty
*/
// Created: JSR 2010-10-18
// =============================================================================
template< typename T >
class ClientObjectProperty : public ClientObjectProperty_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientObjectProperty( const T& value, const std::string& name );
    virtual ~ClientObjectProperty();
    //@}

    //! @name Operations
    //@{
    void Update( const T& value );
    virtual void Serialize( sword::ClientObjectProperty* msg ) const;
    virtual void Write( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientObjectProperty( const ClientObjectProperty& );            //!< Copy constructor
    ClientObjectProperty& operator=( const ClientObjectProperty& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T value_;
    std::string name_;
    static const std::string XMLType_;
    //@}
};

const std::string ClientObjectProperty< std::string >::XMLType_ = "string";
const std::string ClientObjectProperty< int >::XMLType_ = "integer";
const std::string ClientObjectProperty< unsigned int >::XMLType_ = "unsigned-integer";
const std::string ClientObjectProperty< float >::XMLType_ = "float";
const std::string ClientObjectProperty< bool >::XMLType_ = "boolean";

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty constructor
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
template< typename T >
ClientObjectProperty< T >::ClientObjectProperty( const T& value, const std::string& name )
    : value_( value )
    , name_ ( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty destructor
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
template< typename T >
ClientObjectProperty< T >::~ClientObjectProperty()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Update
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
template< typename T >
void ClientObjectProperty< T >::Update( const T& value )
{
    value_ = value;
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Serialize
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObjectProperty< std::string >::Serialize( sword::ClientObjectProperty* msg ) const
{
    msg->set_name( name_ );
    msg->mutable_value()->set_string_value( value_ );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Serialize
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObjectProperty< int >::Serialize( sword::ClientObjectProperty* msg ) const
{
    msg->set_name( name_ );
    msg->mutable_value()->set_integer_value( value_ );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Serialize
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObjectProperty< unsigned int >::Serialize( sword::ClientObjectProperty* msg ) const
{
    msg->set_name( name_ );
    msg->mutable_value()->set_unsigned_integer_value( value_ );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Serialize
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObjectProperty< float >::Serialize( sword::ClientObjectProperty* msg ) const
{
    msg->set_name( name_ );
    msg->mutable_value()->set_float_value( value_ );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Serialize
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObjectProperty< bool >::Serialize( sword::ClientObjectProperty* msg ) const
{
    msg->set_name( name_ );
    msg->mutable_value()->set_bool_value( value_ );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectProperty::Write
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
template< typename T >
void ClientObjectProperty< T >::Write( xml::xostream& xos ) const
{
    xos << xml::start( "property" )
        << xml::attribute( "key", name_ )
        << xml::attribute( "type", XMLType_ )
        << xml::attribute( "value", boost::lexical_cast< std::string >( value_ ) )
        << xml::end;
}

}
}

#endif // __ClientObjectProperty_h_
