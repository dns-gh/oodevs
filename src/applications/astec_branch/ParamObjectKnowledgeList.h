// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObjectKnowledgeList_h_
#define __ParamObjectKnowledgeList_h_

#include "ASN_Types.h"
#include "EntityListParameter.h"

class ObjectKnowledge;
class Object_ABC;

// =============================================================================
/** @class  ParamObjectKnowledgeList
    @brief  ParamObjectKnowledgeList
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledgeList : public EntityListParameter< ObjectKnowledge >
                               , public ContextMenuObserver_ABC< Object_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeObject& asn, const std::string& label, const std::string& menu );
    virtual ~ParamObjectKnowledgeList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamObjectKnowledgeList( const ParamObjectKnowledgeList& );            //!< Copy constructor
    ParamObjectKnowledgeList& operator=( const ParamObjectKnowledgeList& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Object_ABC& entity, QPopupMenu& menu );
    //@}
};

#endif // __ParamObjectKnowledgeList_h_
