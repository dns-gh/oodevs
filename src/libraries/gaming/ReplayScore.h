// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ReplayScore_h_
#define __ReplayScore_h_

#include "protocol/AarSenders.h"
#include "Score.h"

class ScoreDefinition;
class ScoreDefinitions;

// =============================================================================
/** @class  ReplayScore
    @brief  ReplayScore
*/
// Created: SBO 2009-04-30
// =============================================================================
class ReplayScore : public Score
{
public:
    //! @name Constructors/Destructor
    //@{
    ReplayScore( const sword::Indicator& message, const ScoreDefinitions& definitions, kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~ReplayScore();
    //@}

    //! @name Operations
    //@{
    virtual std::string Commit( const T_Parameters& parameters ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void UpdatePlots( const sword::Indicator& message );
    //@}

private:
    //! @name Member data
    //@{
    const ScoreDefinition& definition_;
    //@}
};

#endif // __ReplayScore_h_
