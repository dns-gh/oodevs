// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h $
// $Author: Jvt $
// $Modtime: 11/05/05 15:17 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Posture.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Posture_h_
#define __PHY_RoleInterface_Posture_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

namespace client
{
    class UnitAttributes;
}

class PHY_Posture;

// =============================================================================
// @class  PHY_RoleInterface_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Posture : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Posture RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Posture() {}
    virtual ~PHY_RoleInterface_Posture() {}
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_Posture& GetLastPosture() const = 0;
    virtual const PHY_Posture& GetCurrentPosture() const = 0;
    virtual double GetPostureCompletionPercentage() const = 0;

    virtual void SetPosturePostePrepareGenie() = 0;
    virtual void UnsetPosturePostePrepareGenie() = 0;
    virtual void SetTimingFactor( double rFactor ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    virtual bool HasChanged() const = 0;

    // Override automatic postures
    virtual void EnableDiscreteMode() = 0;
    virtual void DisableDiscreteMode() = 0;
    //@}

    //! @name Installation
    //@{
    virtual bool IsInstalled() const = 0;
    virtual bool IsUninstalled() const = 0;
    virtual void Install() = 0;
    //@}

    //! @name Perception
    //@{
    virtual void SetStealthFactor( double rValue ) = 0;
    //@}

    //! @name Elongation
    //@{
    virtual void SetElongationFactor( double ) = 0;
    virtual double GetElongationFactor() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsStealth() const = 0;
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

#endif // __PHY_RoleInterface_Posture_h_
