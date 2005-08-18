// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __AGR_TesterMissionGenerator_h_
#define __AGR_TesterMissionGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif


#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_TesterMissionGenerator
    @brief  AGR_TesterMissionGenerator
    @par    Using example
    @code
    AGR_TesterMissionGenerator;
    @endcode
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_TesterMissionGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_TesterMissionGenerator();
    virtual ~AGR_TesterMissionGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AGR_TesterMissionGenerator( const AGR_TesterMissionGenerator& );            //!< Copy constructor
    AGR_TesterMissionGenerator& operator=( const AGR_TesterMissionGenerator& ); //!< Assignment operator
    //@}
};

#endif // __AGR_TesterMissionGenerator_h_
