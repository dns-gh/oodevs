// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Message_h_
#define __Message_h_

#include <boost/shared_ptr.hpp>
#include <vector>

namespace boost { 
    namespace asio {
        class const_buffers_1;
        class mutable_buffers_1;
    }
}

namespace tools
{

// =============================================================================
/** @class  Message
    @brief  Message
*/
// Created: AGE 2007-09-05
// =============================================================================
class Message
{

public:
    //! @name Constructors/Destructor
    //@{
              Message();
     explicit Message( unsigned long size );
             ~Message();
    //@}

    //! @name Operations
    //@{
    Message& operator >> ( bool&               n );
    Message& operator >> ( unsigned char&      n );
    Message& operator >> ( char&               n );
    Message& operator >> ( unsigned short&     n );
    Message& operator >> ( short&              n );
    Message& operator >> ( unsigned long&      n );
    Message& operator >> ( long&               n );
    Message& operator >> ( unsigned long long& n );
    Message& operator >> ( long long&          n );
    Message& operator >> ( float&              n );
    Message& operator >> ( double&             n );
    Message& operator >> ( std::string& string );

    Message& operator << ( bool               n );
    Message& operator << ( unsigned char      n );
    Message& operator << ( char               n );
    Message& operator << ( unsigned short     n );
    Message& operator << ( short              n );
    Message& operator << ( unsigned long      n );
    Message& operator << ( long               n );
    Message& operator << ( unsigned long long n );
    Message& operator << ( long long          n );
    Message& operator << ( float              n );
    Message& operator << ( double             n );
    Message& operator << ( const std::string& string );
    Message& operator << ( const char*        pString );

    unsigned long Size() const;
    void Read ( char* data, unsigned size );
    void Write( const char* data, unsigned size );
    //@}

    //! @name Operations
    //@{
    boost::asio::const_buffers_1   MakeOutputBuffer( unsigned long tag ) const;
    boost::asio::mutable_buffers_1 MakeInputBuffer();
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void Write( const T& element )
    {
        Write( reinterpret_cast< const char* >( &element ), sizeof( T ) );
    }
    template< typename T >
    void Read( T& element )
    {
        Read( reinterpret_cast< char* >( &element ), sizeof( T ) );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< char > T_Data;
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< T_Data > data_;
    unsigned readOffset_;
    //@}
};

}

#endif // __Message_h_
