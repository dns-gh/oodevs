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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Object_ABC.h $
// $Author: Age $
// $Modtime: 13/12/04 10:28 $
// $Revision: 4 $
// $Workfile: HLA_Object_ABC.h $
//
// *****************************************************************************

#ifndef __HLA_Object_ABC_h_
#define __HLA_Object_ABC_h_

class AttributeIdentifier;
class UpdateFunctor_ABC;
class Deserializer;
class ObjectIdentifier;

// =============================================================================
/** @class  HLA_Object_ABC
    @brief  Shared object definition
*/
// Created: AGE 2004-11-30
// =============================================================================
class HLA_Object_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Object_ABC();
    virtual ~HLA_Object_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Construct ( double rDeltaPercentage );
    virtual void Destroy   ( double rDeltaPercentage );
    virtual void Mine      ( double rDeltaPercentage );
    virtual void Demine    ( double rDeltaPercentage );
    virtual void Bypass    ( double rDeltaPercentage );
//    virtual void Prepare   ();
    virtual void Activate  ();

    virtual void Deserialize( const AttributeIdentifier& attributeID, const Deserializer& deserializer ) = 0;
    virtual void Serialize  ( UpdateFunctor_ABC& functor, bool bUpdateAll ) const = 0;

    virtual void Destroy() = 0;

    virtual bool HideObject() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual ObjectIdentifier GetId() const = 0;
    //@}
};

#endif // __HLA_Object_ABC_h_
