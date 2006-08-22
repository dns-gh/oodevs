// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionFactory_h_
#define __MissionFactory_h_

namespace kernel
{
    class Mission;
    class FragOrder;

// =============================================================================
/** @class  MissionFactory
    @brief  MissionFactory
*/
// Created: AGE 2006-03-14
// =============================================================================
class MissionFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionFactory();
    virtual ~MissionFactory();
    //@}

    //! @name Operations
    //@{
    virtual Mission*   CreateAgentMission( const std::string& name );
    virtual Mission*   CreateAutomatMission( const std::string& name );
    virtual Mission*   CreatePopulationMission( const std::string& name );
    virtual FragOrder* CreateFragOrder( const std::string& name );
    //@}

private:
    //! @name Helpers
    //@{
    Mission* AddFragOrders( Mission* mission );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionFactory( const MissionFactory& );            //!< Copy constructor
    MissionFactory& operator=( const MissionFactory& ); //!< Assignement operator
    //@}

};

}

#endif // __MissionFactory_h_
