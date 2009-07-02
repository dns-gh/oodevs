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

#include "MT_Tools/MT_Role_ABC.h"

class PHY_Posture;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RoleInterface_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Posture : public MT_Role_ABC
                                , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Posture RoleInterface;

public:
             PHY_RoleInterface_Posture( MT_RoleContainer& role );
             PHY_RoleInterface_Posture();
    virtual ~PHY_RoleInterface_Posture();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    virtual const PHY_Posture& GetLastPosture                () const = 0;
    virtual const PHY_Posture& GetCurrentPosture             () const = 0;
    virtual       MT_Float     GetPostureCompletionPercentage() const = 0;    
    virtual       bool         CanBePerceived                ( const MIL_AgentPion& perceiver ) const = 0;

    virtual       void         SetPosturePostePrepareGenie   () = 0;
    virtual       void         UnsetPosturePostePrepareGenie () = 0;
    virtual       void         SetTimingFactor               ( MT_Float rFactor ) = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Posture::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Posture::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Posture_h_
