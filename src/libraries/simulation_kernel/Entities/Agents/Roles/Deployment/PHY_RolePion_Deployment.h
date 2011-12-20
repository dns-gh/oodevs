// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RolePion_Deployment_h_
#define __PHY_RolePion_Deployment_h_

#include "MIL.h"
#include "PHY_RoleInterface_Deployment.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RolePion_Deployment
    @brief  PHY_RolePion_Deployment
*/
// Created: ABR 2011-12-15
// =============================================================================
class PHY_RolePion_Deployment : public PHY_RoleInterface_Deployment
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RolePion_Deployment( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_Deployment();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Clean();
    virtual void Update( bool bIsDead );
    //@}

    //! @name Deployment
    //@{
    virtual void Deploy();
    virtual void Undeploy();
    virtual bool IsDeployed() const;
    virtual bool IsUndeployed() const;
    //@}

private:
    //! @name Type
    //@{
    enum E_DeploymentState { eDeployed, eUndeployed, eDeploying, eUndeploying };
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_Deployment )
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC& pion_;
    E_DeploymentState    eDeploymentState_;
    double               rDeploymentValue_;
    double               rDeploymentGap_;
    double               rUndeploymentGap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Deployment )

#endif // __PHY_RolePion_Deployment_h_
