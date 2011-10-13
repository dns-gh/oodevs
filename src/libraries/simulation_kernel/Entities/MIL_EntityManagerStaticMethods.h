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

#include "Tools/MIL_Config.h"
#include <xeumeuleu/xml.hpp>
#include <boost/noncopyable.hpp>

class MIL_Time_ABC;

// =============================================================================
/** @class  MIL_EntityManagerStaticMethods
    @brief  MIL_EntityManagerStaticMethods
*/
// Created: RPD 2010-02-08
// =============================================================================
class MIL_EntityManagerStaticMethods : private boost::noncopyable
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
    //! @name Helpers
    //@{
    static void LoadSensors( xml::xistream& xis, const MIL_Time_ABC& time );
    static void LoadMedical( xml::xistream& xis );
    //@}

protected:
    //! @name static Type Initializations
    //@{

    static void InitializeFuneral    ( MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeMedical    ( MIL_Config& config );
    static void InitializeMedicalTreatment( MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeComposantes( MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeWeapons    ( MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeSensors    ( MIL_Config& config, const MIL_Time_ABC& time );
    //@}
};

#endif // __MIL_EntityManagerStaticMethods_h_
