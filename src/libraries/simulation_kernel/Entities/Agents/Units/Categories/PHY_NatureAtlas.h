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

#include "MT_Tools/MT_String.h"

// =============================================================================
// @class  PHY_NatureAtlas
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureAtlas : private boost::noncopyable
{
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
          unsigned int         GetID   () const;
          unsigned int         GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNatureAtlas_Blinde         = 1, // NB : valeurs imposés par l'ASN
        eNatureAtlas_ASS            = 2,
        eNatureAtlas_PC             = 4,
        eNatureAtlas_Log            = 8,
        eNatureAtlas_Mortier        = 16,
        eNatureAtlas_LanceRoquette  = 32,
        eNatureAtlas_Vehicule       = 64,
        eNatureAtlas_PersonnelAPied = 128,
        eNatureAtlas_Helicoptere    = 256,
        eNatureAtlas_Indefini       = 512,
        eNatureAtlas_ASA            = 1024,
        eNatureAtlas_None           = 2048
    };

    typedef std::map< std::string, const PHY_NatureAtlas* > T_NatureAtlasMap;
    typedef T_NatureAtlasMap::const_iterator              CIT_NatureAtlasMap;
    //@}

private:
     PHY_NatureAtlas( const std::string& strName, E_Type nType, unsigned int nAsnID );
    virtual ~PHY_NatureAtlas();

private:
    const std::string  strName_;
    const E_Type       nType_;
    const unsigned int         nAsnID_;

private:
    static T_NatureAtlasMap natureAtlas_;
};

#endif // __PHY_NatureAtlas_h_
