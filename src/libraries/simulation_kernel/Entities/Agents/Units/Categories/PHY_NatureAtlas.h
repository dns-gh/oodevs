// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_NatureAtlas.h $
// $Author: Nld $
// $Modtime: 18/03/05 12:13 $
// $Revision: 4 $
// $Workfile: PHY_NatureAtlas.h $
//
// *****************************************************************************

#ifndef __PHY_NatureAtlas_h_
#define __PHY_NatureAtlas_h_

#include "MIL.h"

#include "network/Simulation_Asn.h"

// =============================================================================
// @class  PHY_NatureAtlas
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureAtlas
{
    MT_COPYNOTALLOWED( PHY_NatureAtlas )

public:
    // default value
    static const PHY_NatureAtlas notDefined_;

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureAtlas* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName () const;
          uint         GetID   () const;
          uint         GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNatureAtlas_Blinde         = 0x8000, // NB : valeurs imposés par l'ASN
        eNatureAtlas_ASS            = 0x4000,
        eNatureAtlas_PC             = 0x2000,
        eNatureAtlas_Log            = 0x1000,
        eNatureAtlas_Mortier        = 0x0800,
        eNatureAtlas_LanceRoquette  = 0x0400,
        eNatureAtlas_Vehicule       = 0x0200,
        eNatureAtlas_PersonnelAPied = 0x0100,
        eNatureAtlas_Helicoptere    = 0x0080,
        eNatureAtlas_Indefini       = 0x0040,
        eNatureAtlas_ASA            = 0x0020,
        eNatureAtlas_None           = 0x0010
    };

    typedef std::map< std::string, const PHY_NatureAtlas*, sCaseInsensitiveLess > T_NatureAtlasMap;
    typedef T_NatureAtlasMap::const_iterator                                      CIT_NatureAtlasMap;
    //@}

private:
     PHY_NatureAtlas( const std::string& strName, E_Type nType, uint nAsnID );
    ~PHY_NatureAtlas();

private:
    const std::string  strName_;
    const E_Type       nType_;
    const uint         nAsnID_;

private:
    static T_NatureAtlasMap natureAtlas_;
};

#include "PHY_NatureAtlas.inl"

#endif // __PHY_NatureAtlas_h_
