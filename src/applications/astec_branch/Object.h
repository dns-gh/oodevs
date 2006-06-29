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

#ifndef __Object_h_
#define __Object_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "Entity_ABC.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"
#include "Drawable_ABC.h"

class Controller;
class Team;
class ObjectType;
class DotationType;
class Displayer_ABC;
class CoordinateConverter_ABC;

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object : public Entity_ABC
             , public Extension_ABC
             , public Updatable_ABC< ASN1T_MsgObjectUpdate >
             , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Object( const ASN1T_MsgObjectCreation& asnMsg, Controller& controller, const CoordinateConverter_ABC& converter,
                     const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< ObjectType >& typeResolver,
                     const Resolver_ABC< DotationType >& dotationResolver );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    bool IsInTeam( const Team& team ) const;
    void Display( Displayer_ABC& displayer ) const;
    void DisplayInTooltip( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
    Object( const Object& );
    Object& operator=( const Object& );
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
    const CoordinateConverter_ABC& converter_;
    ObjectType&   type_;
    unsigned long nId_;
    std::string   strName_;
    Team&         team_;

    float rConstructionPercentage_;
    float rValorizationPercentage_;
    float rBypassConstructionPercentage_;

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

#endif // __Object_h_
