// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreModel_h_
#define __ScoreModel_h_

#include "clients_kernel/Resolver.h"
#include "game_asn/Aar.h"

namespace kernel
{
    class Controller;
}

class Score;
class Publisher_ABC;

// =============================================================================
/** @class  ScoreModel
    @brief  ScoreModel
*/
// Created: SBO 2009-03-12
// =============================================================================
class ScoreModel : public kernel::Resolver< Score, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ScoreModel( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~ScoreModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Update( const ASN1T_MsgIndicator& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreModel( const ScoreModel& );            //!< Copy constructor
    ScoreModel& operator=( const ScoreModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __ScoreModel_h_
