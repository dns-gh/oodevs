// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-07 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __AGR_RC_h_
#define __AGR_RC_h_

class AGR_Workspace;
class AGR_TypeRC;

// =============================================================================
// Created: NLD 2005-09-07
// =============================================================================
class AGR_RC
{
public:
    //! @name Constructors/Destructor
    //@{
     AGR_RC( MT_XXmlInputArchive& input, const AGR_Workspace& workspace );
    ~AGR_RC();
    //@}

    //! @name Generators
    //@{
    std::string GenerateHALEnum                 () const;
    std::string GenerateMILRCClassHeader        () const;
    std::string GenerateMILRCClassBodyInitialize() const;
    std::string GenerateMILRCClassBodyIncludes  () const;
    std::string GenerateMOS2                    () const;
    std::string GenerateTICRCClassBodyInitialize() const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetAsnName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_RC( const AGR_RC& );            //!< Copy constructor
    AGR_RC& operator=( const AGR_RC& ); //!< Assignement operator
    //@}

private:
    std::string strAsnName_;
    const AGR_TypeRC* pType_;
};


#include "AGR_RC.inl"

#endif // __AGR_RC_h_
