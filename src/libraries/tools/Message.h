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

namespace boost
{
namespace asio
{
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
     explicit Message( std::size_t size );
     virtual ~Message();
    //@}

    //! @name Operations
    //@{
    Message& operator >>( unsigned long& n );

    std::size_t Size() const;
    const char* Data() const;

    void Read ( char* data, std::size_t size );
    void Write( const char* data, std::size_t size );
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
    std::size_t readOffset_;
    //@}
};

}

#endif // __Message_h_
