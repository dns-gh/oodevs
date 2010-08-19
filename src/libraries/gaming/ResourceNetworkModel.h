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

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

class ResourceNetwork;
class ResourceNetworkFactory;

class UrbanResourceResolver : public tools::Resolver< ResourceNetwork >
{
public:
    UrbanResourceResolver() : tools::Resolver< ResourceNetwork >() {}
};

class ObjectResourceResolver : public tools::Resolver< ResourceNetwork >
{
public:
    ObjectResourceResolver() : tools::Resolver< ResourceNetwork >() {}
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
