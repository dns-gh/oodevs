// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Hla/HLA_RoleLocalPion.h $
// $Author: Age $
// $Modtime: 24/11/04 16:01 $
// $Revision: 3 $
// $Workfile: HLA_RoleLocalPion.h $
//
// *****************************************************************************

#ifndef __HLA_RoleLocalPion_h_
#define __HLA_RoleLocalPion_h_

#include "HLA_RoleInterface.h"

class MIL_AgentPion;
namespace hla 
{
    class ObjectIdenfifier;
}


// =============================================================================
/** @class  HLA_RoleLocalPion
    @brief  HLA role local pion
*/
// Created: AGE 2004-11-10
// =============================================================================
class HLA_RoleLocalPion : public HLA_RoleInterface
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_RoleLocalPion( MT_RoleContainer& roleContainer, MIL_AgentPion& pion );
    virtual ~HLA_RoleLocalPion();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, const hla::Deserializer& deserializer );
    //@}

    //! @name Accessors
    //@{
    virtual hla::ObjectIdentifier GetId() const;
    //@}

    //! @name Modifiers
    //@{
    void SetId( const hla::ObjectIdentifier& objectId );
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&          pion_;
    hla::ObjectIdentifier*  pObjectId_;
    //@}
};

#endif // __HLA_RoleLocalPion_h_
