// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __JoinAnalysis_h_
#define __JoinAnalysis_h_

#include "SpawnCommand.h"

namespace frontend
{

// =============================================================================
/** @class  JoinAnalysis
    @brief  JoinAnalysis
*/
// Created: AGE 2007-10-05
// =============================================================================
class JoinAnalysis : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             JoinAnalysis( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& profile, unsigned port, bool attach = false );
    virtual ~JoinAnalysis();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    JoinAnalysis( const JoinAnalysis& );            //!< Copy constructor
    JoinAnalysis& operator=( const JoinAnalysis& ); //!< Assignment operator
    //@}
};

}

#endif // __JoinAnalysis_h_
