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
             JoinAnalysis( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, unsigned number );
    virtual ~JoinAnalysis();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    JoinAnalysis( const JoinAnalysis& );            //!< Copy constructor
    JoinAnalysis& operator=( const JoinAnalysis& ); //!< Assignment operator
    //@}
};

#endif // __JoinAnalysis_h_
