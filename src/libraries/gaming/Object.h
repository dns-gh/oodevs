// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "ASN_Types.h"
#include "clients_kernel/IDManager.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Team_ABC;
    class ObjectType;
    class DotationType;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
    class InstanciationComplete;
}

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object : public kernel::Object_ABC
             , public kernel::Extension_ABC
             , public kernel::Updatable_ABC< kernel::InstanciationComplete >
             , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
             , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Object( const ASN1T_MsgObjectCreation& asnMsg, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                     const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver, const kernel::Resolver_ABC< kernel::ObjectType >& typeResolver,
                     const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    virtual const kernel::Team_ABC& GetTeam() const;
    virtual kernel::ObjectType& GetType() const;
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
    virtual void DoUpdate( const  kernel::InstanciationComplete& );
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller&   controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::ObjectType&   type_;
    unsigned long nId_;
    QString   strName_;
    kernel::Team_ABC&     team_;

    float rConstructionPercentage_;
    float rValorizationPercentage_;
    float rBypassConstructionPercentage_;

    bool bPrepared_;
    //@}

public:
    //! @name Member data
    //@{
    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    kernel::DotationType* construction_;
    kernel::DotationType* valorization_;
    unsigned int nDotationConstruction_;
    unsigned int nDotationValorization_;
    //@}
};

#endif // __Object_h_
