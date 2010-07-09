// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Bool_h_
#define __MissionParameter_Bool_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Bool
    @brief  MissionParameter_Bool
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Bool : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_Bool( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Bool();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Bool( const MissionParameter_Bool& );            //!< Copy constructor
    MissionParameter_Bool& operator=( const MissionParameter_Bool& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool bValue_;
    //@}
};

}

#endif // __MissionParameter_Bool_h_
