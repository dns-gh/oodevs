// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorConditions_h_
#define __SuccessFactorConditions_h_

#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Score_ABC;
class ScoresModel;
class SuccessFactorCondition;

// =============================================================================
/** @class  SuccessFactorConditions
    @brief  SuccessFactorConditions
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorConditions : public tools::Resolver< SuccessFactorCondition >
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< Score_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SuccessFactorConditions( kernel::Controllers& controllers );
             SuccessFactorConditions( kernel::Controllers& controllers, xml::xistream& xis, const ScoresModel& model );
    virtual ~SuccessFactorConditions();
    //@}

    //! @name Accessors
    //@{
    std::string GetOperator() const;
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void SetOperator( const std::string& op );
    void AddCondition( SuccessFactorCondition& condition );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorConditions( const SuccessFactorConditions& );            //!< Copy constructor
    SuccessFactorConditions& operator=( const SuccessFactorConditions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const Score_ABC& score );
    void ReadCondition( xml::xistream& xis, const ScoresModel& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::string operator_;
    //@}
};

#endif // __SuccessFactorConditions_h_
