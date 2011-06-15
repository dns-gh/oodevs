// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MissionController_h_
#define __MissionController_h_

#include "MissionController_ABC.h"
#include "MIL.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MIL_AgentPion;

// =============================================================================
/** @class  MissionController
    @brief  Mission controller
*/
// Created: LGY 2011-06-14
// =============================================================================
class MissionController : public MissionController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionController();
    virtual ~MissionController();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( AgentFactory_ABC& factory );
    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< MIL_Mission_ABC >   T_Mission;
    typedef std::map< unsigned int, T_Mission >    T_Missions;
    //@}

private:
    //! @name Member data
    //@{
    T_Missions missions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MissionController )

#endif // __MissionController_h_
