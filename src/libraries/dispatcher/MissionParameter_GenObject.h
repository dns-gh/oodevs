// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_GenObject_h_
#define __MissionParameter_GenObject_h_

#include "MissionParameter_ABC.h"
#include "GenObject.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_GenObject
    @brief  MissionParameter_GenObject
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_GenObject : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_GenObject( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_GenObject();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_GenObject( const MissionParameter_GenObject& );            //!< Copy constructor
    MissionParameter_GenObject& operator=( const MissionParameter_GenObject& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    GenObject data_;
    //@}
};

}

#endif // __MissionParameter_GenObject_h_
