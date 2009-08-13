// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Posture_h_
#define __PHY_RoleHLA_Posture_h_

#include "PHY_RoleInterface_Posture.h"
#include "hla/Deserializer.h"

namespace hla 
{
    class AttributeIdentifier;
}

class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RoleHLA_Posture
    @brief  HLA role posture
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Posture : public PHY_RoleInterface_Posture
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Posture( const MIL_Agent_ABC& pion );
    virtual ~PHY_RoleHLA_Posture();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Posture& GetLastPosture                () const;
    virtual const PHY_Posture& GetCurrentPosture             () const;
    virtual       MT_Float     GetPostureCompletionPercentage() const;    
    virtual       bool         CanBePerceived                ( const MIL_AgentPion& perceiver ) const;
    virtual       void         SetPosturePostePrepareGenie   ();
    virtual       void         UnsetPosturePostePrepareGenie ();
    virtual       void         SetTimingFactor               ( MT_Float rFactor );
    
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_Posture* pPosture_;
    const MIL_Agent_ABC& pion_;
    bool bIsStealth_;
    bool bTransported_;
    //@}
};

#endif // __PHY_RoleHLA_Posture_h_
