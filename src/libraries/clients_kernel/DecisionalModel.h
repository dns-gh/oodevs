// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DecisionalModel_h_
#define __DecisionalModel_h_

#include "tools/Resolver.h"

namespace xml { class xistream; };

namespace kernel
{
    class Mission;
    class MissionFactory;
    class FragOrder;
    class FragOrderType;

// =============================================================================
/** @class  DecisionalModel
    @brief  DecisionalModel
*/
// Created: AGE 2006-02-14
// =============================================================================
class DecisionalModel : public  tools::Resolver< Mission >
                      , public  tools::Resolver< FragOrder >
{
public:
    //! @name Types
    //@{
    typedef Mission* (MissionFactory::*T_Resolver)( const std::string& );
    //@}
public:
    //! @name Constructors/Destructor
    //@{
             DecisionalModel( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver, const tools::Resolver_ABC< FragOrderType >& fragOrders );
    virtual ~DecisionalModel();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecisionalModel( const DecisionalModel& );            //!< Copy constructor
    DecisionalModel& operator=( const DecisionalModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadMission( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver );
    void ReadFragOrder( xml::xistream& xis, Mission& mission, MissionFactory& factory );
    void ReadDefaultFragOrder( xml::xistream& xis, MissionFactory& factory, const tools::Resolver_ABC< FragOrderType >& fragorders );
    void RegisterFragOrder( MissionFactory& factory, const FragOrderType& type );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    //@}
};

}

#endif // __DecisionalModel_h_
