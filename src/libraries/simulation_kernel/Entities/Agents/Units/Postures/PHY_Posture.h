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
#include "ENT/ENT_Enums.h"

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
    //! @name Postures
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

    static std::size_t GetPostureCount();
    static const PHY_Posture* FindPosture( const std::string& strName );
    static const PHY_Posture* FindPosture( unsigned int nID );
    static const PHY_Posture& FindPosture( sword::UnitAttributes_Posture nAsnID );
    //@}

    //! @name Accessors
    //@{
    const std::string&                  GetName               () const;
          unsigned int                  GetID                 () const;
          sword::UnitAttributes_Posture GetAsnID              () const;
    const PHY_Posture*                  GetNextAutoPosture    () const;
    const PHY_Posture*                  GetPreviousAutoPosture() const;
    const PHY_ConsumptionType&          GetConsumptionMode    () const;

    bool CanModifyPH       () const;
    bool CanModifyDetection() const;
    bool IsInstantaneous   () const;
    bool operator < ( const PHY_Posture& p ) const;
    bool operator == ( const PHY_Posture& p ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_PostureFlags
    {
        ePostureCanModifyPH        = 0x01,
        ePostureCanModifyDetection = 0x02,
        ePostureInstantaneous      = 0x04
    };
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     PHY_Posture( const std::string& strName, E_UnitPosture nType, sword::UnitAttributes_Posture nAsnID, const PHY_ConsumptionType& consumptionMode, unsigned int nFlags, const PHY_Posture* pNextAutoPosture = 0, const PHY_Posture* pPreviousAutoPosture = 0 );
    ~PHY_Posture();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                   strName_;
    const unsigned int                  nType_;
    const sword::UnitAttributes_Posture nAsnID_;
    const unsigned int                  nFlags_;
    const PHY_ConsumptionType&          consumptionMode_;
    const PHY_Posture*                  pNextAutoPosture_;
    const PHY_Posture*                  pPreviousAutoPosture_;
    //@}
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const PHY_Posture* t )
    {
        unsigned int id = t ? t->GetID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const PHY_Posture*& t )
    {
        unsigned int id;
        ar >> id;
        t = PHY_Posture::FindPosture( id );
    }
}
}

#endif // __PHY_Posture_h_
