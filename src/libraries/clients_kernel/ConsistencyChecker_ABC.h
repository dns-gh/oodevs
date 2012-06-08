// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsistencyChecker_ABC_h_
#define __ConsistencyChecker_ABC_h_

#include <boost/noncopyable.hpp>
#include "SafePointer.h"

namespace kernel
{

// =============================================================================
/** @class  ConsistencyChecker_ABC
    @brief  ConsistencyChecker_ABC
*/
// Created: ABR 2012-06-06
// =============================================================================
class ConsistencyChecker_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ConsistencyChecker_ABC() {}
    virtual ~ConsistencyChecker_ABC() {}
    //@}

    //! @name Abstract operation
    //@{
    virtual bool CheckConsistency() = 0;
    //@}
};

// =============================================================================
/** @class  ConsistencyChecker
    @brief  ConsistencyChecker
*/
// Created: ABR 2012-06-06
// =============================================================================
template< typename EnumError, typename Item >
class ConsistencyChecker : public ConsistencyChecker_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConsistencyChecker();
    virtual ~ConsistencyChecker();
    //@}

    //! @name Abstract operation
    //@{
    virtual bool CheckConsistency() = 0;
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< Item* >                 T_Items;
    typedef typename T_Items::iterator          IT_Items;
    typedef typename T_Items::const_iterator   CIT_Items;

    struct ConsistencyError
    {
        ConsistencyError( EnumError type ) : type_( type ) {}

        EnumError type_;
        T_Items items_;
        std::string optional_;
    };
    //@}

    //! @name Typedef
    //@{
    typedef std::vector< ConsistencyError >                 T_ConsistencyErrors;
    typedef typename T_ConsistencyErrors::iterator         IT_ConsistencyErrors;
    typedef typename T_ConsistencyErrors::const_iterator  CIT_ConsistencyErrors;
    //@}

    //! @name Operations
    //@{
    const T_ConsistencyErrors& GetConsistencyErrors() const { return errors_; }
    //@}

protected:
    //! @name Helpers
    //@{
    void AddError( EnumError type, Item* item, const std::string& optional = "" );
    void ClearErrors();
    //@}

protected:
    //! @name Member data
    //@{
    T_ConsistencyErrors  errors_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ConsistencyChecker constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename Item >
ConsistencyChecker< EnumError, Item >::ConsistencyChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsistencyChecker destructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename Item >
ConsistencyChecker< EnumError, Item >::~ConsistencyChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsistencyChecker::AddError
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename Item >
void ConsistencyChecker< EnumError, Item >::AddError( EnumError type, Item* item, const std::string& optional /*= ""*/ )
{
    ConsistencyError error( type );
    error.items_.push_back( item );
    error.optional_ = optional;
    errors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyChecker::ClearErrors
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError, typename Item >
void ConsistencyChecker< EnumError, Item >::ClearErrors()
{
    for( IT_ConsistencyErrors it = errors_.begin(); it != errors_.end(); ++it )
        for( IT_Items item = it->items_.begin(); item != it->items_.end(); ++item )
            delete *item;
    errors_.clear();
}

}

#endif // __ConsistencyChecker_ABC_h_
