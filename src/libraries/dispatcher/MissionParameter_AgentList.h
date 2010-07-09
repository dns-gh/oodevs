// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_AgentList_h_
#define __MissionParameter_AgentList_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_AgentList
    @brief  MissionParameter_AgentList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_AgentList : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_AgentList( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_AgentList();
    //@}

    //! @name Operations
    //@{
    virtual void Send  ( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_AgentList( const MissionParameter_AgentList& );            //!< Copy constructor
    MissionParameter_AgentList& operator=( const MissionParameter_AgentList& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< int > agents_;
    //@}
};

}

#endif // __MissionParameter_AgentList_h_
