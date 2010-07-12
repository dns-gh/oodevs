// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterHeading_h_
#define __MissionParameterHeading_h_

#include "MissionParameter_ABC.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Agent_ABC;
    class Automat_ABC;
}

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  MissionParameterHeading
    @brief  MissionParameterHeading
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameterHeading : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterHeading( xml::xistream& xis, const kernel::OrderParameter& type, const dispatcher::Agent_ABC& agent );
             MissionParameterHeading( xml::xistream& xis, const kernel::OrderParameter& type, const dispatcher::Automat_ABC& automat );
    virtual ~MissionParameterHeading();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( MsgMissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterHeading( const MissionParameterHeading& );            //!< Copy constructor
    MissionParameterHeading& operator=( const MissionParameterHeading& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned short angle_;
    //@}
};

}
}

#endif // __MissionParameterHeading_h_
