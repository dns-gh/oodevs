// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Object_ABC_h_
#define __Object_ABC_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "FireResult.h"
#include "Entity_ABC.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Controller;
class Team;
class ObjectType;

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object_ABC : public Entity_ABC
                 , private Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgObjectUpdate >
                 , public Updatable_ABC< ASN1T_MsgExplosion >
                 , public Updatable_ABC< ASN1T_FireDamagesPion >
{

public:
    //! @name Constructors/Destructor
    //@{
             Object_ABC( const ASN1T_MsgObjectCreation& asnMsg, Controller& controller,
                         const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< ObjectType >& typeResolver );
    virtual ~Object_ABC();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    const Team& GetTeam() const;
    ObjectType& GetType() const;
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    Object_ABC( const Object_ABC& );
    Object_ABC& operator=( const Object_ABC& );
    //@}

private:
    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgExplosion& message );
    virtual void DoUpdate( const ASN1T_FireDamagesPion& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller&   controller_;
    ObjectType&   type_;
    unsigned long nId_;
    std::string   strName_;
    Team&         team_;

    T_PointVector pointVector_;
    MT_Vector2D   center_;

    MT_Float rConstructionPercentage_;
    MT_Float rValorizationPercentage_;
    MT_Float rBypassConstructionPercentage_;

    bool bPrepared_;
//    T_FireResults              explosionResults_;
    //@}

private:
    //! @name Member data
    //@{
    

    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    // Dotations
    // $$$$ AGE 2006-02-15: 
//    std::string                strTypeDotationConstruction_;
//    std::string                strTypeDotationValorization_;
//    uint                       nNbrDotationConstruction_;
//    uint                       nNbrDotationValorization_;
    //@}
};

#endif // __Object_ABC_h_
