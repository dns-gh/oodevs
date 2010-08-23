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

// =============================================================================
/** @class  ResourceNetworkModel
    @brief  Resource network model
*/
// Created: JSR 2010-08-13
// =============================================================================
class ResourceNetworkModel : private tools::Resolver< NodeProperties >
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
    static EResourceType FindResourceType( const std::string& type );
    void Update();
    void RegisterNode( NodeProperties& nodeProperties, unsigned int id );
    void Push( unsigned int id, int quantity, EResourceType type ) const;
    //@}
};

}

#endif // __ResourceNetworkModel_h_
