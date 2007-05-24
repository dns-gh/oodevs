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

#include "game_asn/Asn.h"
#include "ObjectKnowledge_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
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
                      , public kernel::Displayable_ABC
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

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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

    kernel::OptionalValue< unsigned int >   nConstructionPercentage_;
    kernel::OptionalValue< unsigned int >   nValorizationPercentage_;
    kernel::OptionalValue< unsigned int >   nBypassConstructionPercentage_;
    kernel::OptionalValue< bool >           bIsPerceived_;
    kernel::OptionalValue< E_TypeObstacle > obstacleType_;
    kernel::OptionalValue< bool           > reservedObstacleActivated_;
    kernel::OptionalValue< uint >           nRelevance_;
    // $$$$ NLD 2007-05-23: Manque les dotations
    //@}
};

#endif // __ObjectKnowledge_h_
