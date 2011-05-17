// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TypeDispatcher_h_
#define __TypeDispatcher_h_

#include "Position.h"
#include "Zone.h"
#include "Types.h"
#include <boost/noncopyable.hpp>
#include <xeumeuleu/xml.hpp>

class Task;

// =============================================================================
/** @class  TypeDispatcher
    @brief  TypeDispatcher
*/
// Created: AGE 2007-10-08
// =============================================================================
class TypeDispatcher : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct NullType
    {
        NullType() {}
        template< typename T > explicit NullType( const T& ) {}
        template< typename T > NullType& operator+=( const T& )       { return *this; }
        template< typename T > NullType  operator+ ( const T& ) const { return *this; }
        template< typename T > NullType& operator-=( const T& )       { return *this; }
        template< typename T > NullType  operator- ( const T& ) const { return *this; }
        template< typename T > NullType& operator/=( const T& )       { return *this; }
        template< typename T > NullType  operator/ ( const T& ) const { return *this; }
        template< typename T > bool operator==( const T& ) const { return false; }
        template< typename T > bool operator!=( const T& ) const { return false; }
        template< typename T > bool operator<( const T& ) const { return false; }
        template< typename T > bool operator>( const T& ) const { return false; }
        operator double() const { return 0.; }
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    TypeDispatcher( const std::string& name, xml::xistream& xis, Task& result, bool allowNull = false )
        : name_( name ), xis_( xis ), result_( result ), allowNullType_( allowNull ) {}
    TypeDispatcher( xml::xistream& xis, Task& result, bool allowNull = false )
        : name_( xis.attribute< std::string >( "id" ) ), xis_( xis ), result_( result ), allowNullType_( allowNull ) {}
    virtual ~TypeDispatcher() {}
    //@}

    //! @name Operations
    //@{
    template< typename Functor >
    void Dispatch( Functor& functor, const std::string& type )
    {
        if( type == "float" )
            functor.operator()< NumericValue >( name_, xis_, result_ );
        else if( type == "position" )
            functor.operator()< ::Position >( name_, xis_, result_ );
        else if( type == "zone" )
            functor.operator()< ::Zone >( name_, xis_, result_ );
        else if( type == "bool" )
            functor.operator()< bool >( name_, xis_, result_ );
        else if( type == "int" )
            functor.operator()< NumericValue >( name_, xis_, result_ );
        else if( type == "unsigned" || type == "unsigned int" )
            functor.operator()< NumericValue >( name_, xis_, result_ );
        else if( type == "unsigned long" || type == "unit" )
            functor.operator()< NumericValue >( name_, xis_, result_ );
        else if( type.empty() && allowNullType_ )
            functor.operator()< NullType >( name_, xis_, result_ );
        else
            TypeError( type );
    }
    template< typename Functor >
    void Dispatch( Functor& functor )
    {
        Dispatch( functor, xml::attribute( xis_, "type", std::string() ) );
    }
    //@}

private:
    //! @name Helpers
    //@{
    void TypeError( const std::string& type )
    {
        throw std::runtime_error( "Unknown type '" + type + "'" );
    }
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    xml::xistream& xis_;
    Task& result_;
    bool allowNullType_;
    //@}
};

inline
std::istream& operator>>( std::istream& stream, TypeDispatcher::NullType& ) { return stream; }

#endif // __TypeDispatcher_h_
