// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_RoleDistantPion_h_
#define __HLA_RoleDistantPion_h_

#include "HLA_RoleInterface.h"
#include <hla/ObjectIdentifier.h>

class MIL_AgentHLA;

// =============================================================================
/** @class  HLA_RoleDistantPion
    @brief  HLA role distant pion
*/
// Created: AGE 2004-11-10
// =============================================================================
class HLA_RoleDistantPion : public HLA_RoleInterface
{

public:
    //! @name Constructors/Destructor
    //@{
            HLA_RoleDistantPion( MT_RoleContainer& roleContainer, MIL_AgentHLA& pion, const hla::ObjectIdentifier& objectId );
    virtual ~HLA_RoleDistantPion();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize  ( hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, const hla::Deserializer& deserializer );
    //@}

    //! @name Accessors
    //@{
    virtual hla::ObjectIdentifier GetId() const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentHLA& pion_;
    hla::ObjectIdentifier objectId_;
    //@}
};

#endif // __HLA_RoleDistantPion_h_
