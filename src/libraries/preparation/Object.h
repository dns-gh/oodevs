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

#include "clients_kernel/IDManager.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class ObjectType;
    class DotationType;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

class IdManager;

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object : public kernel::EntityImplementation< kernel::Object_ABC >
             , public kernel::Extension_ABC
             , public kernel::Drawable_ABC
             , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Object( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                     const kernel::ObjectType& type, IdManager& idManager );
             Object( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, 
                     const kernel::Resolver_ABC< kernel::ObjectType, QString >& types, IdManager& idManager );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::ObjectType& GetType() const;
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );
    Object& operator=( const Object& );
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    static unsigned long             ReadId  ( xml::xistream& xis );
    static QString                   ReadName( xml::xistream& xis );
    static const kernel::ObjectType& ReadType( xml::xistream& xis, const kernel::Resolver_ABC< kernel::ObjectType, QString >& types );
    //@}

public:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::ObjectType& type_;

    float rConstructionPercentage_;
    float rValorizationPercentage_;
    float rBypassConstructionPercentage_;
    bool bPrepared_;

    kernel::DotationType* construction_;
    kernel::DotationType* valorization_;
    unsigned int nDotationConstruction_;
    unsigned int nDotationValorization_;
    //@}
};

#endif // __Object_h_
