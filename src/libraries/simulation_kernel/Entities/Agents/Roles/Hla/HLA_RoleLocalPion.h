// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_RoleLocalPion_h_
#define __HLA_RoleLocalPion_h_

#include "HLA_RoleInterface.h"

class MIL_AgentPion;

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
    explicit HLA_RoleLocalPion( MIL_AgentPion& pion );
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
    hla::ObjectIdentifier*  pObjectId_;
    //@}
};

#endif // __HLA_RoleLocalPion_h_
