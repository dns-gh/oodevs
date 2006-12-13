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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_XmlGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 11:09 $
// $Revision: 1 $
// $Workfile: AGR_XmlGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_XmlGenerator_h_
#define __AGR_XmlGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_XmlGenerator
    @brief  AGR_XmlGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_XmlGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_XmlGenerator();
    virtual ~AGR_XmlGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_XmlGenerator( const AGR_XmlGenerator& );            //!< Copy constructor
    AGR_XmlGenerator& operator=( const AGR_XmlGenerator& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void GenerateMissions( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateRCs     ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}
};

#endif // __AGR_XmlGenerator_h_
