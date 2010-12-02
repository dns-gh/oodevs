// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_EntityManagerStaticMethods_h_
#define __MIL_EntityManagerStaticMethods_h_

#include <xeumeuleu/xml.hpp>
#include "Tools/MIL_Config.h"

class MIL_Time_ABC;

// =============================================================================
/** @class  MIL_EntityManagerStaticMethods
    @brief  MIL_EntityManagerStaticMethods
*/
// Created: RPD 2010-02-08
// =============================================================================
class MIL_EntityManagerStaticMethods
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_EntityManagerStaticMethods();
    virtual ~MIL_EntityManagerStaticMethods();
    //@}

    //! @name static Initialization
    //@{
    static void Initialize( MIL_Config& config, const MIL_Time_ABC& time );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_EntityManagerStaticMethods( const MIL_EntityManagerStaticMethods& );            //!< Copy constructor
    MIL_EntityManagerStaticMethods& operator=( const MIL_EntityManagerStaticMethods& ); //!< Assignment operator
    //@}

protected:
    //! @name static Type Initializations
    //@{
    template < typename T >
    static void InitializeType       ( xml::xistream& xis, MIL_Config& config, const std::string& strSection );
    static void InitializeMedical    ( xml::xistream& xis, MIL_Config& config );
    static void InitializeMedicalTreatment( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeComposantes( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeWeapons    ( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeSensors    ( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time );
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeType
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
template < typename T >
void MIL_EntityManagerStaticMethods::InitializeType( xml::xistream& xis, MIL_Config& config, const std::string& strSection )
{
    std::string strFile;
    xis >> xml::start( strSection )
            >> xml::attribute( "file", strFile )
        >> xml::end;

    strFile = config.BuildPhysicalChildFile( strFile );
    MIL_Tools::CheckXmlCrc32Signature( strFile );
    xml::xifstream xisType( strFile );
    config.AddFileToCRC( strFile );

    T::Initialize( xisType );//verfifier tous les initialize
}

#endif // __MIL_EntityManagerStaticMethods_h_
