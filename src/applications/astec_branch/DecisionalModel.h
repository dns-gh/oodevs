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

#include "Resolver_ABC.h"

namespace xml { class xistream; };

// =============================================================================
/** @class  DecisionalModel
    @brief  DecisionalModel
*/
// Created: AGE 2006-02-14
// =============================================================================
class DecisionalModel
{
public:
    //! @name Types
    //@{
    typedef unsigned long (*T_Resolver)( const std::string& );
    //@}
public:
    //! @name Constructors/Destructor
    //@{
             DecisionalModel( xml::xistream& xis, const T_Resolver& missionResolver, const T_Resolver& fragOrderResolver);
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

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_Missions;
    typedef std::set   < unsigned long >   T_FragOrders;
    //@}

    //! @name Helpers
    //@{
    void ReadMission( xml::xistream& xis, const T_Resolver& missionResolver, const T_Resolver& fragOrderResolver );
    void ReadFragOrder( xml::xistream& xis, const T_Resolver& resolver );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    T_Missions   availableMissions_;
    T_FragOrders availableFragOrders_;
    //@}
};

#endif // __DecisionalModel_h_
