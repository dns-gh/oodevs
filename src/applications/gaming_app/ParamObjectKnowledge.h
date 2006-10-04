// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObjectKnowledge_h_
#define __ParamObjectKnowledge_h_

#include "EntityParameter.h"

namespace kernel
{
    class Object_ABC;
    class Entity_ABC;
}

class ObjectKnowledge;
class ObjectKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamObjectKnowledge
    @brief  ParamObjectKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledge : public EntityParameter< ObjectKnowledge >
                           , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledge( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamObjectKnowledge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamObjectKnowledge( const ParamObjectKnowledge& );            //!< Copy constructor
    ParamObjectKnowledge& operator=( const ParamObjectKnowledge& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name Member data
    //@{
    ObjectKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

#endif // __ParamObjectKnowledge_h_
