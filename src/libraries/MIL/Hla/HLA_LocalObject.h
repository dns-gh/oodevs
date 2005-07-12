// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-30 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_LocalObject.h $
// $Author: Age $
// $Modtime: 13/12/04 10:29 $
// $Revision: 5 $
// $Workfile: HLA_LocalObject.h $
//
// *****************************************************************************

#ifndef __HLA_LocalObject_h_
#define __HLA_LocalObject_h_

#include "HLA_Object_ABC.h"
#include "hla/ObjectIdentifier.h"

class MIL_RealObject_ABC;

// =============================================================================
/** @class  HLA_LocalObject
    @brief  HLA local object
*/
// Created: AGE 2004-11-30
// =============================================================================
class HLA_LocalObject : public HLA_Object_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_LocalObject( MIL_RealObject_ABC& object );
    virtual ~HLA_LocalObject();
    //@}

    //! @name Operations
    //@{
    virtual void Deserialize( const AttributeIdentifier& attributeID, const Deserializer& deserializer );
    virtual void Serialize  ( UpdateFunctor_ABC& functor, bool bUpdateAll ) const;

    virtual void Destroy();

    virtual bool HideObject() const;
    //@}

    //! @name Accessors
    //@{
    virtual ObjectIdentifier GetId() const;
    //@}

    //! @name Modifiers 
    //@{
    void SetId( const ObjectIdentifier& objectId );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_LocalObject( const HLA_LocalObject& );            //!< Copy constructor
    HLA_LocalObject& operator=( const HLA_LocalObject& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_RealObject_ABC& object_;
    ObjectIdentifier id_;
    //@}
};

#endif // __HLA_LocalObject_h_
