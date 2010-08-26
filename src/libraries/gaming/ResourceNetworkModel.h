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

#include "tools/Resolver.h"

namespace kernel
{
    class ResourceNetwork_ABC;
}

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

class ResourceNetworkFactory;

class UrbanResourceResolver : public tools::Resolver< kernel::ResourceNetwork_ABC >
{
public:
    UrbanResourceResolver() : tools::Resolver< kernel::ResourceNetwork_ABC >() {}
};

class ObjectResourceResolver : public tools::Resolver< kernel::ResourceNetwork_ABC >
{
public:
    ObjectResourceResolver() : tools::Resolver< kernel::ResourceNetwork_ABC >() {}
};

// =============================================================================
/** @class  ResourceNetworkModel
    @brief  ResourceNetworkModel
*/
// Created: JSR 2010-08-18
// =============================================================================
class ResourceNetworkModel : public UrbanResourceResolver
                           , public ObjectResourceResolver
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkModel( ResourceNetworkFactory& factory );
    virtual ~ResourceNetworkModel();
    //@}

    //! @name Operations
    //@{
    void UrbanCreate( kernel::Entity_ABC& entity, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg );
    void UrbanUpdate( unsigned int id, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkModel( const ResourceNetworkModel& );            //!< Copy constructor
    ResourceNetworkModel& operator=( const ResourceNetworkModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ResourceNetworkFactory& factory_;
    //@}
};

#endif // __ResourceNetworkModel_h_
