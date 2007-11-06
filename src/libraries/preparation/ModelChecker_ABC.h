// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelChecker_ABC_h_
#define __ModelChecker_ABC_h_

// =============================================================================
/** @class  ModelChecker_ABC
    @brief  ModelChecker_ABC
*/
// Created: SBO 2007-11-06
// =============================================================================
class ModelChecker_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ModelChecker_ABC() {}
    virtual ~ModelChecker_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Validate() = 0;
    virtual bool Reject( const QString& reason ) = 0;
    virtual bool Prompt( const QString& question ) = 0;
    //@}
};

#endif // __ModelChecker_ABC_h_
