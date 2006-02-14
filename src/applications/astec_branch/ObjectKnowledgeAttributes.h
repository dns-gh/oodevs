// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeAttributes_h_
#define __ObjectKnowledgeAttributes_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Controller;
class Object_ABC;

// =============================================================================
/** @class  ObjectKnowledgeAttributes
    @brief  ObjectKnowledgeAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledgeAttributes : public Extension_ABC
                                , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                                , public Updatable_ABC< ASN1T_MsgObjectKnowledgeCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeAttributes( Controller& controller, const Resolver_ABC< Object_ABC >& resolver );
    virtual ~ObjectKnowledgeAttributes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeAttributes( const ObjectKnowledgeAttributes& );            //!< Copy constructor
    ObjectKnowledgeAttributes& operator=( const ObjectKnowledgeAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void Update( const ASN1T_MsgObjectKnowledgeCreation& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Object_ABC >& resolver_;
    T_PointVector points_;
    Object_ABC* pRealObject_;

    OptionalValue< unsigned int >  nPourcentageConstruction_;
    OptionalValue< unsigned int >  nPourcentageValorisation_;
    OptionalValue< unsigned int >  nPourcentageContournement_;
    OptionalValue< bool >          bEnPreparation_;
    OptionalValue< bool >          bIsPerceived_;
    OptionalValue< uint >          nRelevance_;
    //@}
};

#endif // __ObjectKnowledgeAttributes_h_
