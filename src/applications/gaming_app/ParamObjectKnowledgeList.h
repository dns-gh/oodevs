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

#include "gaming/ASN_Types.h"
#include "EntityListParameter.h"

namespace kernel
{
    class Object_ABC;
}

class ObjectKnowledge;

// =============================================================================
/** @class  ParamObjectKnowledgeList
    @brief  ParamObjectKnowledgeList
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledgeList : public EntityListParameter< ObjectKnowledge >
                               , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeObject& asn, const QString& label, const QString& menu );
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
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}
};

#endif // __ParamObjectKnowledgeList_h_
