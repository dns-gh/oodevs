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

#include "Resolver.h"

namespace xml { class xistream; };
class Mission;
class MissionFactory;
class FragOrder;

// =============================================================================
/** @class  DecisionalModel
    @brief  DecisionalModel
*/
// Created: AGE 2006-02-14
// =============================================================================
class DecisionalModel : public Resolver< Mission >
                      , public Resolver< FragOrder >
{
public:
    //! @name Types
    //@{
    typedef Mission* (MissionFactory::*T_Resolver)( const std::string& );
    //@}
public:
    //! @name Constructors/Destructor
    //@{
             DecisionalModel( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver );
    virtual ~DecisionalModel();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DecisionalModel( const DecisionalModel& );            //!< Copy constructor
    DecisionalModel& operator=( const DecisionalModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadMission( xml::xistream& xis, MissionFactory& factory, const T_Resolver& missionResolver );
    void ReadFragOrder( xml::xistream& xis, Mission& mission, MissionFactory& factory );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    //@}
};

#endif // __DecisionalModel_h_
