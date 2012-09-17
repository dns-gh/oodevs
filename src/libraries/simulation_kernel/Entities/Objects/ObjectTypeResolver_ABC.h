// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ObjectTypeResolver_ABC_h
#define ObjectTypeResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

class MIL_ObjectType_ABC;

// =============================================================================
/** @class  ObjectTypeResolver_ABC
    @brief  Object type resolver declaration
*/
// Created: LGY 2012-09-14
// =============================================================================
class ObjectTypeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectTypeResolver_ABC() {}
    virtual ~ObjectTypeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MIL_ObjectType_ABC& FindType( const std::string& type ) const = 0;
    //@}
};

#endif // ObjectTypeResolver_ABC_h
