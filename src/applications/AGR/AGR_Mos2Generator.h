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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Mos2Generator.h $
// $Author: Age $
// $Modtime: 25/10/04 11:55 $
// $Revision: 3 $
// $Workfile: AGR_Mos2Generator.h $
//
// *****************************************************************************

#ifndef __AGR_Mos2Generator_h_
#define __AGR_Mos2Generator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_Mos2Generator
    @brief  AGR_Mos2Generator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_Mos2Generator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_Mos2Generator();
    virtual ~AGR_Mos2Generator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Mos2Generator( const AGR_Mos2Generator& );            //!< Copy constructor
    AGR_Mos2Generator& operator=( const AGR_Mos2Generator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMos2MissionInterfaceHeaderFiles( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMos2MissionInterfaceCppFiles   ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMos2EnumConverterFiles         ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMos2Rcs                        ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    //@}
};

#endif // __AGR_Mos2Generator_h_
