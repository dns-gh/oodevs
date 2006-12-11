// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledge_h_
#define __ObjectKnowledge_h_

#include "ASN_Types.h"
#include "ObjectKnowledge_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/EntityImplementation.h"

namespace kernel
{
    class Displayer_ABC;
    class Controller;
    class Agent_ABC;
    class CoordinateConverter_ABC;
    class ObjectType;
}

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledge : public kernel::EntityImplementation< ObjectKnowledge_ABC >
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                      , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const kernel::Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message,
                              kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                              const kernel::Resolver_ABC< kernel::Object_ABC >& objectResolver,
                              const kernel::Resolver_ABC< kernel::ObjectType >& typeResolver );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual QString GetTypeName() const;
    virtual const kernel::Object_ABC* GetEntity() const;
    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const kernel::Team_ABC&   GetOwner() const;

    void Display( kernel::Displayer_ABC& displayer ) const;
    void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Resolver_ABC< kernel::Object_ABC >& objectResolver_;
    const kernel::Team_ABC& owner_;

    const kernel::ObjectType* type_;

    std::string position_;
    kernel::Object_ABC* pRealObject_;

    kernel::OptionalValue< unsigned int >  nPourcentageConstruction_;
    kernel::OptionalValue< unsigned int >  nPourcentageValorisation_;
    kernel::OptionalValue< unsigned int >  nPourcentageContournement_;
    kernel::OptionalValue< bool >          bEnPreparation_;
    kernel::OptionalValue< bool >          bIsPerceived_;
    kernel::OptionalValue< uint >          nRelevance_;
    //@}
};

#endif // __ObjectKnowledge_h_
