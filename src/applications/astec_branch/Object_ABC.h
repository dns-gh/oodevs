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

class Controller;
namespace xml { class xistream; };

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
    explicit Object_ABC( const ASN1T_MsgObjectCreation& asnMsg, Controller& controller );
    virtual ~Object_ABC();
    //@}

    //! @name Static Operations
    //@{
    static void           InitializeObjectIds      ( xml::xistream& xis );
    static IDManager& GetIDManagerForObjectType( ASN1T_EnumObjectType nType );
    static IDManager& GetIDManagerForObjectType( uint );

    // $$$$ AGE 2006-02-15: Crap ! Change Updatable::Update to Updatable::Something ?
    template< typename T >
    void UpdateObject( const T& message ) {
        Entity_ABC::Update( message );
    }
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;
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
    virtual void Update( const ASN1T_MsgObjectUpdate& message );
    virtual void Update( const ASN1T_MsgExplosion& message );
    virtual void Update( const ASN1T_FireDamagesPion& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller&          controller_;

    ASN1T_EnumObjectType nType_;
    unsigned long        nId_;
    std::string          strName_;

    T_PointVector        pointVector_;
    MT_Vector2D          center_;

    MT_Float rConstructionPercentage_;
    MT_Float rValorizationPercentage_;
    MT_Float rBypassConstructionPercentage_;

    bool bPrepared_;
//    T_FireResults              explosionResults_;
    //@}

private:
    //! @name Member data
    //@{
//    Team*                  pTeam_; // $$$$ AGE 2006-02-15: 

    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    // Dotations
    // $$$$ AGE 2006-02-15: 
//    std::string                strTypeDotationConstruction_;
//    std::string                strTypeDotationValorization_;
//    uint                       nNbrDotationConstruction_;
//    uint                       nNbrDotationValorization_;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< ASN1T_EnumObjectType, unsigned int >  T_ObjectIDs;
    typedef std::map< unsigned int, IDManager* >        T_Managers;
    //@}

public:
    //! @name Static members
    //@{
    static T_ObjectIDs objectIds_;
    static T_Managers  managers_;
    //@}
};

#endif // __Object_ABC_h_
