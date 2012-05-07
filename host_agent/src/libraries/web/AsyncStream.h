// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ASYNC_STREAM_H
#define ASYNC_STREAM_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace web
{
// =============================================================================
/** @class  AsyncStream
    @brief  AsyncStream class definition
*/
// Created: BAX 2012-05-03
// =============================================================================
class AsyncStream
{
public:
    //! @name Constructors/Destructor
    //@{
             AsyncStream();
    virtual ~AsyncStream();
    //@}

    //! @name Writer Methods
    //@{
    void Write( const char* data, size_t size );
    void CloseWrite();
    //@}

    //! @name Writer Methods
    //@{
    typedef boost::function< void( std::istream& ) > Handler;
    void Read( Handler handler );
    //@}

private:
    struct Private;
    boost::shared_ptr< Private > private_;
};
}

#endif // ASYNC_STREAM_H
