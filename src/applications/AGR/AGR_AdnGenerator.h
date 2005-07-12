// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_AdnGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 11:48 $
// $Revision: 1 $
// $Workfile: AGR_AdnGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_AdnGenerator_h_
#define __AGR_AdnGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

class AGR_Mission;

// =============================================================================
/** @class  AGR_AdnGenerator
    @brief  AGR_AdnGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_AdnGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_AdnGenerator();
    virtual ~AGR_AdnGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_AdnGenerator( const AGR_AdnGenerator& );            //!< Copy constructor
    AGR_AdnGenerator& operator=( const AGR_AdnGenerator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool IsOfType( const AGR_Mission& mission, const std::string& strType ) const;
    void GenerateAdnEnumFile        ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateAdnMissionConverter( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    //@}
};

#endif // __AGR_AdnGenerator_h_
