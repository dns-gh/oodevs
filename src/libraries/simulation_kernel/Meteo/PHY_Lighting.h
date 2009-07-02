// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Lighting.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_Lighting.h $
//
// *****************************************************************************

#ifndef __PHY_Lighting_h_
#define __PHY_Lighting_h_

#include "MIL.h"

// =============================================================================
// @class  PHY_Lighting
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Lighting : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Lighting*, sCaseInsensitiveLess > T_LightingMap;
    typedef T_LightingMap::const_iterator                                       CIT_LightingMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_LightingMap& GetLightings();
    static const PHY_Lighting* FindLighting( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
          uint         GetID  () const;
    //@}

    //! @name Operations
    //@{
    const PHY_Lighting& GetDegradedLighting( uint nDegradationFactor ) const;
    //@}

public:
    static PHY_Lighting jourSansNuage_;
    static PHY_Lighting jourPeuNuageux_;
    static PHY_Lighting jourMoyennementNuageux_;
    static PHY_Lighting jourAssezNuageux_;
    static PHY_Lighting jourTresNuageux_;
    static PHY_Lighting nuitPleineLune_;
    static PHY_Lighting nuitTroisQuartDeLune_;
    static PHY_Lighting nuitDemiLune_;
    static PHY_Lighting nuitQuartDeLune_;
    static PHY_Lighting nuitNouvelleLune_;
    static PHY_Lighting eclairant_;

private:
    //! @name Types
    //@{
    enum E_Lighting
    {
        eJourSansNuage,
        eJourPeuNuageux,
        eJourMoyennementNuageux,
        eJourAssezNuageux,
        eJourTresNuageux,

        eNuitPleineLune,
        eNuitTroisQuartDeLune,
        eNuitDemiLune,
        eNuitQuartDeLune,
        eNuitNouvelleLune,

        eEclairant
    };
    //@}

private:
     PHY_Lighting( const std::string& strName, E_Lighting nType, const PHY_Lighting* pNextDegradedLighting );
    ~PHY_Lighting();

private:
    const std::string    strName_;
    const E_Lighting     nType_;
    const PHY_Lighting* pNextDegradedLighting_;

private:
    static T_LightingMap lightings_;
};

#include "PHY_Lighting.inl"

#endif // __PHY_Lighting_h_
