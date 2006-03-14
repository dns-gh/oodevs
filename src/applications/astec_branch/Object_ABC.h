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
#include "Entity_ABC.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Controller;
class Team;
class ObjectType;
class DotationType;
class Displayer_ABC;
class CoordinateConverter;

// =============================================================================
// Created: SBO 2005-09-02
// $$$$ AGE 2006-02-16: renommmer en Object
// =============================================================================
class Object_ABC : public Entity_ABC
                 , private Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgObjectUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             Object_ABC( const ASN1T_MsgObjectCreation& asnMsg, Controller& controller, const CoordinateConverter& converter,
                         const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< ObjectType >& typeResolver,
                         const Resolver_ABC< DotationType >& dotationResolver );
    virtual ~Object_ABC();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
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
    //@}

public:
    //! @name Member data
    //@{
    Controller&   controller_;
    const CoordinateConverter& converter_;
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
    //@}

public:
    //! @name Member data
    //@{
    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    DotationType* construction_;
    DotationType* valorization_;
    unsigned int nDotationConstruction_;
    unsigned int nDotationValorization_;
    //@}
};

#endif // __Object_ABC_h_
