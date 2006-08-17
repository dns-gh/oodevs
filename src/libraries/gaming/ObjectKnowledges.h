// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledges_h_
#define __ObjectKnowledges_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver.h"

class ObjectKnowledge;
class Controller;
class ObjectKnowledgeFactory;
class Team_ABC;

// =============================================================================
/** @class  ObjectKnowledges
    @brief  ObjectKnowledges
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledges : public Extension_ABC
                       , public Updatable_ABC< ASN1T_MsgObjectKnowledgeCreation >
                       , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                       , public Updatable_ABC< ASN1T_MsgObjectKnowledgeDestruction >
                       , public Resolver< ObjectKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledges( const Team_ABC& team, Controller& controller, ObjectKnowledgeFactory& factory );
    virtual ~ObjectKnowledges();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledges( const ObjectKnowledges& );            //!< Copy constructor
    ObjectKnowledges& operator=( const ObjectKnowledges& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeCreation&    message );
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate&      message );
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    const Team_ABC& team_;
    Controller& controller_;
    ObjectKnowledgeFactory& factory_;
    //@}
};

#endif // __ObjectKnowledges_h_
