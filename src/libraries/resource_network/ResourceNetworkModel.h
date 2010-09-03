// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkModel_h_
#define __ResourceNetworkModel_h_

#include "NodeProperties.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace resource
{

class UrbanResolver : public tools::Resolver< NodeProperties >
{
public:
    UrbanResolver() : tools::Resolver< NodeProperties >() {}
};

class ObjectResolver : public tools::Resolver< NodeProperties >
{
public:
    ObjectResolver() : tools::Resolver< NodeProperties >() {}
};

// =============================================================================
/** @class  ResourceNetworkModel
    @brief  Resource network model
*/
// Created: JSR 2010-08-13
// =============================================================================
class ResourceNetworkModel : private UrbanResolver
                           , private ObjectResolver
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkModel();
    virtual ~ResourceNetworkModel();
    //@}

    //! @name Operations
    //@{
    void Update();
    void RegisterNode( NodeProperties& nodeProperties, unsigned int id, bool urban );
    void Push( unsigned int id, bool urban, int quantity, unsigned long resourceId ) const;
    //@}
};

}

#endif // __ResourceNetworkModel_h_
