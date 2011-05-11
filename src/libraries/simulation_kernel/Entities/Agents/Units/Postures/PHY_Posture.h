// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Postures/PHY_Posture.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:50 $
// $Revision: 4 $
// $Workfile: PHY_Posture.h $
//
// *****************************************************************************

#ifndef __PHY_Posture_h_
#define __PHY_Posture_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include "MT_Tools/MT_String.h"

namespace sword
{
    enum UnitAttributes_Posture;
}

class PHY_ConsumptionType;

// =============================================================================
// @class  PHY_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Posture : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Posture* > T_PostureMap;
    typedef T_PostureMap::const_iterator              CIT_PostureMap;
    //@}

public:
    //! @name
    //@{
    static const PHY_Posture mouvement_;
    static const PHY_Posture mouvementDiscret_;
    static const PHY_Posture arret_;
    static const PHY_Posture posteReflexe_;
    static const PHY_Posture poste_;
    static const PHY_Posture posteAmenage_;
    static const PHY_Posture postePrepareGenie_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_PostureMap& GetPostures();
    static const PHY_Posture*  FindPosture( const std::string& strName );
    static const PHY_Posture*  FindPosture( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&                                  GetName           () const;
          unsigned int                                  GetID             () const;
          sword::UnitAttributes_Posture    GetAsnID          () const;
    const PHY_Posture*                                  GetNextAutoPosture() const;
    const PHY_ConsumptionType&                          GetConsumptionMode() const;

    bool CanModifyPH       () const;
    bool CanModifyDetection() const;
    bool IsInstantaneous   () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_PostureType
    {
        ePostureMouvement         = 0,
        ePostureMouvementDiscret  = 1,
        ePostureArret             = 2,
        ePosturePosteReflexe      = 3,
        ePosturePoste             = 4,
        ePosturePosteAmenage      = 5,
        ePosturePostePrepareGenie = 6
    };

    enum E_PostureFlags
    {
        ePostureCanModifyPH        = 0x01,
        ePostureCanModifyDetection = 0x02,
        ePostureInstantaneous      = 0x04
    };
    //@}

private:
     PHY_Posture( const std::string& strName, E_PostureType nType, sword::UnitAttributes_Posture nAsnID, const PHY_ConsumptionType& consumptionMode, unsigned int nFlags, const PHY_Posture* pNextAutoPosture = 0 );
    ~PHY_Posture();

private:
    const std::string                                   strName_;
    const E_PostureType                                 nType_;
    const sword::UnitAttributes_Posture    nAsnID_;
    const unsigned int                                  nFlags_;
    const PHY_ConsumptionType&                          consumptionMode_;
    const PHY_Posture*                                  pNextAutoPosture_;

private:
    static T_PostureMap                                 postures_;
};

#endif // __PHY_Posture_h_
