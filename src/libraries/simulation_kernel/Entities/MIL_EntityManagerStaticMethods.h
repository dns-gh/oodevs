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

namespace tools
{
    class PhyLoader;
}

class MIL_Time_ABC;
class MIL_ObjectFactory;
class ObjectTypeResolver_ABC;

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
    static void Initialize( const tools::PhyLoader& loader, const MIL_Time_ABC& time, MIL_ObjectFactory& objectFactory );
    //@}

private:
    //! @name Helpers
    //@{
    static void LoadSensors( xml::xistream& xis, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver );
    static void LoadMedical( xml::xistream& xis );
    //@}

protected:
    //! @name static Type Initializations
    //@{

    static void InitializeFuneral         ( const tools::PhyLoader& loader, const MIL_Time_ABC& time );
    static void InitializeMedical         ( const tools::PhyLoader& loader );
    static void InitializeComposantes     ( const tools::PhyLoader& loader, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver );
    static void InitializeWeapons         ( const tools::PhyLoader& loader, const MIL_Time_ABC& time );
    static void InitializeObjects         ( const tools::PhyLoader& loader, MIL_ObjectFactory& objectFactory );
    static void InitializeSensors         ( const tools::PhyLoader& loader, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver );
    //@}
};

#endif // __MIL_EntityManagerStaticMethods_h_
