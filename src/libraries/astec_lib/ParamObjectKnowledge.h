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

class ObjectKnowledge;
class Object;

// =============================================================================
/** @class  ParamObjectKnowledge
    @brief  ParamObjectKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamObjectKnowledge : public EntityParameter< ObjectKnowledge >
                           , public ContextMenuObserver_ABC< Object >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamObjectKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu );
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
    virtual void NotifyContextMenu( const Object& entity, QPopupMenu& menu );
    //@}
};

#endif // __ParamObjectKnowledge_h_
