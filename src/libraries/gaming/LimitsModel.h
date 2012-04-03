// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsModel_h_
#define __LimitsModel_h_

#include "tools/Resolver.h"

namespace sword
{
    class LimitCreation;
    class PhaseLineCreation;
}

class TacticalLine_ABC;
class TacticalLineFactory;

// =============================================================================
/** @class  LimitsModel
    @brief  LimitsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class LimitsModel : public tools::Resolver< TacticalLine_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LimitsModel( TacticalLineFactory& factory );
    virtual ~LimitsModel();
    //@}

    //! @name Operations
    //@{
    virtual void Create( const sword::LimitCreation& message );
    virtual void Create( const sword::PhaseLineCreation& message );

    void DeleteLimit( unsigned long id );
    void DeleteLima( unsigned long id );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LimitsModel( const LimitsModel& );            //!< Copy constructor
    LimitsModel& operator=( const LimitsModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TacticalLineFactory& factory_;
    //@}
};

#endif // __LimitsModel_h_
