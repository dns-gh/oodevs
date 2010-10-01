// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_LocalObject_h_
#define __HLA_LocalObject_h_

#include "HLA_Object_ABC.h"
#include <hla/ObjectIdentifier.h>

class MIL_Object_ABC;

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
             HLA_LocalObject( MIL_Object_ABC& object );
    virtual ~HLA_LocalObject();
    //@}

    //! @name Operations
    //@{
    virtual void Construct( double rDeltaPercentage );
    virtual void Destroy( double rDeltaPercentage );
    virtual void Mine( double rDeltaPercentage );
    virtual void Demine( double rDeltaPercentage );
    virtual void Bypass( double rDeltaPercentage );
//    virtual void Prepare();
    virtual void Activate();
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, const hla::Deserializer& deserializer );
    virtual void Serialize( hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void Destroy();
    virtual bool HideObject() const;
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
    //! @name Copy/Assignment
    //@{
    HLA_LocalObject( const HLA_LocalObject& );            //!< Copy constructor
    HLA_LocalObject& operator=( const HLA_LocalObject& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_Object_ABC& object_;
    hla::ObjectIdentifier id_;
    //@}
};

#endif // __HLA_LocalObject_h_
