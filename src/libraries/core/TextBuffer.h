// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

#include "Serialization.h"
#include <cassert>
#include <cstring>
#include <string>

namespace core
{
// =============================================================================
/** @class  TextBuffer
    @brief  Zero-copy buffer when possible

    TextBuffer purpose is to enable lookups without allocating any strings, when
    we already have some useable char pointer available

    This class has two states:

    * lightweight state where only raw_ & size_ is known, data_ is empty and
      unused. Copying lightweight TextBuffers only copy pointers and not its
      content

    * store mode where raw_ points to data_ and data_ keeps its own copy of the
      string. Copying such TextBuffers should copy its data too. Users should
      always call TextBuffer::Store when inserting keys into the model in order
      to make sure it will keep a persistent copy of its data

*/
// Created: BAX 2012-10-10
// =============================================================================
class TextBuffer
{
public:
    //! @name Constructors/Destructor
    //@{
    TextBuffer()
        : raw_ ( 0 )
        , size_( 0 )
    {
        // NOTHING
    }

    TextBuffer( const char* raw, size_t len )
        : raw_ ( raw )
        , size_( len )
    {
        // NOTHING
    }

    TextBuffer( const TextBuffer& rhs )
        : data_( rhs.data_ )
        , raw_ ( data_.empty() ? rhs.raw_ : data_.c_str() )
        , size_( rhs.size_ )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Store()
    {
        if( data_.size() == size_ )
            return;
        data_.assign( raw_, size_ );
        raw_ = data_.c_str();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< class Archive >
    void load( Archive& archive, const unsigned int /*version*/ )
    {
        archive & data_;
        raw_ = data_.c_str();
        size_ = data_.size();
    }

    template< class Archive >
    void save( Archive& archive, const unsigned int /*version*/ ) const
    {
        archive & data_;
    }
    //@}

    //! @name Accessors
    //@{
    const std::string& String() const
    {
        assert( data_.size() == size_ );
        return data_;
    }
    //@}

    //! @name Operators
    //@{
    TextBuffer& operator=( const TextBuffer& rhs )
    {
        data_ = rhs.data_;
        raw_ = data_.empty() ? rhs.raw_ : data_.c_str();
        size_ = rhs.size_;
        return *this;
    }

    bool operator<( const TextBuffer& rhs ) const
    {
        const size_t next = std::min( size_, rhs.size_ );
        const int cmp = std::strncmp( raw_, rhs.raw_, next );
        if( cmp )
            return cmp < 0;
        return size_ < rhs.size_;
    }

    bool operator==( const TextBuffer& rhs ) const
    {
        if( size_ != rhs.size_ )
            return false;
        if( raw_ == rhs.raw_ )
            return true;
        return !std::strncmp( raw_, rhs.raw_, size_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::string data_;
    const char* raw_;
    size_t size_;
    //@}
};

}

#endif // TEXT_BUFFER_H
