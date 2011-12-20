// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Deployment_h_
#define __PHY_RoleInterface_Deployment_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

// =============================================================================
// @class  PHY_RoleInterface_Deployment
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Deployment : public tools::Role_ABC
                                , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Deployment RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Deployment() {}
    virtual ~PHY_RoleInterface_Deployment() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    //@}

    //! @name Deployment
    //@{
    virtual void Deploy() = 0;
    virtual void Undeploy() = 0;
    virtual bool IsDeployed() const = 0;
    virtual bool IsUndeployed() const = 0;
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_Deployment_h_
