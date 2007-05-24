// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentList_h_
#define __ParamAgentList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Agent_ABC;
    class OrderParameter;
}

// =============================================================================
/** @class  ParamAgentList
    @brief  ParamAgentList
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAgentList : public EntityListParameter< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller );
    virtual ~ParamAgentList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgentList( const ParamAgentList& );            //!< Copy constructor
    ParamAgentList& operator=( const ParamAgentList& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Agent_ABC >* CreateElement( const kernel::Agent_ABC& potential );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    unsigned int count_;
    //@}
};

#endif // __ParamAgentList_h_
