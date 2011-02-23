// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Loader_h__
#define __Loader_h__

#include "Loader_ABC.h"

namespace tools
{
    class ExerciseConfig;

// =============================================================================
/** @class  Loader
    @brief  Loader definition
*/
// Created: NLD 2011-02-23
// =============================================================================
class Loader : public Loader_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Loader( const ExerciseConfig& config );
    virtual ~Loader();
    //@}

    //! @name Operations
    //@{
    virtual void CheckFile                ( const std::string& file ) const;
    virtual void LoadPhysicalFile         ( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const;
    virtual void LoadPhysicalFileAndCRC   ( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const;
    virtual void LoadAndUpdateExerciseFile( const std::string& rootTag, T_Loader loader, const std::string& xslFile ) const;
    virtual void LoadExerciseFileAndCRC   ( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const;
    //@}

private:
    const ExerciseConfig& config_;
};

}

#endif // __Loader_h__
