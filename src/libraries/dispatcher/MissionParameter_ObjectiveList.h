// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_ObjectiveList_h_
#define __MissionParameter_ObjectiveList_h_

#include "MissionParameter_ABC.h"
#include "Objective.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_ObjectiveList
    @brief  MissionParameter_ObjectiveList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_ObjectiveList : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_ObjectiveList( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_ObjectiveList();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_ObjectiveList( const MissionParameter_ObjectiveList& );            //!< Copy constructor
    MissionParameter_ObjectiveList& operator=( const MissionParameter_ObjectiveList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Objective > T_ObjectiveVector;
    //@}

private:
    //! @name Member data
    //@{
    T_ObjectiveVector objects_;
    //@}
};

}

#endif // __MissionParameter_ObjectiveList_h_
