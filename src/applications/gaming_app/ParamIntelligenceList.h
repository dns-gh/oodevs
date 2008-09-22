// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamIntelligenceList_h_
#define __ParamIntelligenceList_h_

#include "EntityListParameter.h"
#include "clients_kernel/Intelligence_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class OrderParameter;
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ParamIntelligenceList
    @brief  ParamIntelligenceList
*/
// Created: SBO 2007-10-24
// =============================================================================
class ParamIntelligenceList : public EntityListParameter< kernel::Intelligence_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamIntelligenceList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamIntelligenceList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamIntelligenceList( const ParamIntelligenceList& );            //!< Copy constructor
    ParamIntelligenceList& operator=( const ParamIntelligenceList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Intelligence_ABC >* CreateElement( const kernel::Intelligence_ABC& potential );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    unsigned int count_;
    //@}
};

#endif // __ParamIntelligenceList_h_
