// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Line_h_
#define __MissionParameter_Line_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

////using namespace Common;

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Line
    @brief  MissionParameter_Line
*/
// Created: SBO 2008-03-04
// =============================================================================
class MissionParameter_Line : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_Line( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Line();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Line( const MissionParameter_Line& );            //!< Copy constructor
    MissionParameter_Line& operator=( const MissionParameter_Line& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    //@}
};

}

#endif // __MissionParameter_Line_h_
