// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __CapacityRetriever_h_
#define __CapacityRetriever_h_

#include <boost/noncopyable.hpp>

class MIL_ObjectType_ABC;
class ObjectCapacity_ABC;

// =============================================================================
/** @class  CapacityRetriever
    @brief  CapacityRetriever
*/
// Created: JSR 2012-04-17
// =============================================================================
class CapacityRetriever : private boost::noncopyable
{
public:
    //! @name Operations
    //@{
    static const ObjectCapacity_ABC* RetrieveCapacity( const MIL_ObjectType_ABC& type, const std::string& capacity );
    //@}
};

#endif // __CapacityRetriever_h_
