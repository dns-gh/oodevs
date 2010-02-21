// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjectiveList_h_
#define __ActionParameterObjectiveList_h_

#include "Parameter.h"

namespace Common
{
    class MsgMissionObjectiveList;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObjectiveList
    @brief  ObjectiveList
*/
// Created: SBO 2007-05-14
// =============================================================================
class ObjectiveList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectiveList( const kernel::OrderParameter& parameter );
             ObjectiveList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             ObjectiveList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgMissionObjectiveList& asn );
    virtual ~ObjectiveList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& asn ) const;
    virtual void Clean( Common::MsgMissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    ObjectiveList( const ObjectiveList& );            //!< Copy constructor
    ObjectiveList& operator=( const ObjectiveList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterObjectiveList_h_
