// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Iterator_h_
#define __Iterator_h_

#include <boost/shared_ptr.hpp>

// =============================================================================
/** @class  Iterator_ABC
    @brief  Iterator definition
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename Item >
class Iterator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Iterator_ABC() {};
    virtual ~Iterator_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool HasMoreElements() const = 0;
    virtual Item NextElement() = 0;
    //@}
};

// =============================================================================
/** @class  Iterator
    @brief  Iterator wrapper
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename Item >
class Iterator : public Iterator_ABC< Item >
{
public:
    /*implicit*/ Iterator( Iterator_ABC< Item >* base ) : base_( base ) {};
    virtual bool HasMoreElements() const { return base_->HasMoreElements(); };
    virtual Item NextElement() { return base_->NextElement(); };
private:
    boost::shared_ptr< Iterator_ABC< Item > > base_;
};

// =============================================================================
/** @class  SimpleIterator
    @brief  Simple container iterator
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename Item, typename Container >
class SimpleIterator : public Iterator_ABC< Item >
{
public:
    SimpleIterator( const Container& container ) 
        : current_( container.begin() )
        , end_( container.end() )
    {}
    virtual bool HasMoreElements() const {
        return current_ != end_;
    };
    virtual Item NextElement() {
        return *current_++;
    }
private:
    SimpleIterator( const SimpleIterator& );
    SimpleIterator& operator=( SimpleIterator& );
    typename Container::const_iterator current_;
    typename Container::const_iterator end_;
};

// =============================================================================
/** @class  AssociativeIterator
    @brief  Associative container iterator
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename Item, typename Container >
class AssociativeIterator : public Iterator_ABC< Item >
{
public:
    AssociativeIterator( const Container& container ) 
        : current_( container.begin() )
        , end_( container.end() )
    {}
    virtual bool HasMoreElements() const {
        return current_ != end_;
    };
    virtual Item NextElement() {
        return *(current_++)->second;
    }
private:
    AssociativeIterator( const AssociativeIterator& );
    AssociativeIterator& operator=( AssociativeIterator& );
    typename Container::const_iterator current_;
    typename Container::const_iterator end_;
};

#endif // __Iterator_h_
