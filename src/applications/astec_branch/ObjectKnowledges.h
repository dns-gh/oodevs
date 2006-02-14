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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class ObjectKnowledge;

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
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledges();
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

    //! @name Types
    //@{
    typedef std::map< unsigned long, ObjectKnowledge* > T_ObjectKnowledges;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    virtual void Update( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    virtual void Update( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    T_ObjectKnowledges knowledgeGroups_;
    //@}
};

#endif // __ObjectKnowledges_h_
