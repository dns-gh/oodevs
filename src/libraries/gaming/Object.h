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

#include "game_asn/Asn.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/OptionalValue.h"

namespace kernel
{
    class Controller;
    class ObjectType;
    class DotationType;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object : public kernel::EntityImplementation< kernel::Object_ABC >
             , public kernel::Extension_ABC
             , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
             , public kernel::Drawable_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Object( const ASN1T_MsgObjectCreation& asnMsg, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                     const kernel::Resolver_ABC< kernel::ObjectType >& typeResolver, const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Accessors
    //@{
    virtual kernel::ObjectType& GetType() const;
    virtual QString GetTypeName() const;
    bool IsPrepared() const;
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

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::ObjectType&   type_;

    kernel::OptionalValue< float > rConstructionPercentage_;
    kernel::OptionalValue< float > rValorizationPercentage_;
    kernel::OptionalValue< float > rBypassConstructionPercentage_;

    kernel::OptionalValue< bool >  bPrepared_;
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    kernel::DotationType* construction_;
    kernel::DotationType* valorization_;
    kernel::OptionalValue< unsigned int > nDotationConstruction_;
    kernel::OptionalValue< unsigned int > nDotationValorization_;
    //@}
};

#endif // __Object_h_
