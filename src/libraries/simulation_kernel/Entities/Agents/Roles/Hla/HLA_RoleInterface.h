// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Hla/HLA_RoleInterface.h $
// $Author: Age $
// $Modtime: 24/11/04 15:26 $
// $Revision: 3 $
// $Workfile: HLA_RoleInterface.h $
//
// *****************************************************************************

#ifndef __HLA_RoleInterface_h_
#define __HLA_RoleInterface_h_

#include "MT_Tools/MT_Role_ABC.h"

namespace hla {
    class AttributeIdentifier;    
    class Deserializer;
    class ObjectIdentifier;
    class UpdateFunctor_ABC;
}

// =============================================================================
/** @class  HLA_RoleInterface
    @brief  HLA role interface
*/
// Created: AGE 2004-11-05
// =============================================================================
class HLA_RoleInterface : public MT_Role_ABC
{

public:
    //! @name Types
    //@{
    typedef HLA_RoleInterface RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             HLA_RoleInterface( MT_RoleContainer& roleContainer );
    virtual ~HLA_RoleInterface();
    //@}

    //! @name Operations
    //@{
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, const hla::Deserializer& deserializer ) = 0;
    virtual void Serialize  ( hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const = 0;

    void Destroy();
    //@}

    //! @name Accessors
    //@{
    virtual hla::ObjectIdentifier GetId() const = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_RoleInterface( const HLA_RoleInterface& );            //!< Copy constructor
    HLA_RoleInterface& operator=( const HLA_RoleInterface& ); //!< Assignement operator
    //@}
};

#endif // __HLA_RoleInterface_h_
