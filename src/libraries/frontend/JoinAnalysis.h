// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
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
             JoinAnalysis( const tools::GeneralConfig& config, const tools::Path& exercise,
                     const tools::Path& session, const QString& profile,
                     const QString& features );
    virtual ~JoinAnalysis();
    //@}
};

}

#endif // __JoinAnalysis_h_
