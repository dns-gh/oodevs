// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_Convoy_ABC.h $
// $Author: Jvt $
// $Modtime: 1/04/05 11:44 $
// $Revision: 5 $
// $Workfile: PHY_Convoy_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_Convoy_ABC_h_
#define __PHY_Convoy_ABC_h_

#include "MT_Tools/MT_InterpolatedFunction.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class PHY_SupplyConsign_ABC;
class PHY_ComposantePion;
class MIL_AgentTypePion;
class MIL_MissionType_ABC;
class MIL_Automate;
class MIL_AutomateLOG;
class PHY_Conveyor;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_Convoy_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Convoy_ABC : private boost::noncopyable
{
public:
    //! @name
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

public:
    explicit PHY_Convoy_ABC( PHY_SupplyConsign_ABC& consign );
             PHY_Convoy_ABC();
    virtual ~PHY_Convoy_ABC();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    bool ReserveTransporters();
    double ModifySpeed( double rSpeed ) const;
    //@}

    //! @name Accessors
    //@{
    MIL_AutomateLOG&     GetSupplier() const;
    MIL_AutomateLOG&     GetConvoyer() const;
    const MIL_Automate*  GetNextSupplied() const;
    MIL_Automate&        GetStockSupplier() const;
    unsigned int         GetFormingTime() const;
    unsigned int         GetLoadingTime() const;
    unsigned int         GetUnloadingTime() const;
    const T_PointVector* GetWayPointsToGoToNextSupplied() const;
    const T_PointVector* GetWayPointsToGoBack        () const;
    //@}

    //! @name Events
    //@{
    void EmptyOut();
    void NotifyConveyorDestroyed( PHY_ComposantePion& composante );
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< PHY_ComposantePion*, PHY_Conveyor* > T_ConveyorMap;
    typedef T_ConveyorMap::const_iterator                CIT_ConveyorMap;
    //@}

protected:
    PHY_SupplyConsign_ABC* pConsign_;
    T_ConveyorMap conveyors_;

private:
    //! @name Tools
    //@{
    void UnlockConvoy();
    //@}

private:
    //! @name Initialization
    //@{
    static void InitializeConvoyUnitType( xml::xistream& xis );
    static void InitializeConvoyMission ( xml::xistream& xis );

    static void InitializeInterpolatedTime ( xml::xistream& xis, const std::string& strTagName, MT_InterpolatedFunction< double >& data );
    static void InitializeSpeedModificators( xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction< double >& data, std::pair< unsigned int, double >& upperBound );
    static void ReadSpeedModifier( xml::xistream& xis, std::pair< unsigned int, double >& upperBound );
    //@}

    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, double > T_MerchandiseToConvoyMap;
    typedef T_MerchandiseToConvoyMap::iterator               IT_MerchandiseToConvoyMap;
    //@}

protected:
    static MT_InterpolatedFunction< double > formingTime_;
    static MT_InterpolatedFunction< double > loadingTime_;
    static MT_InterpolatedFunction< double > unloadingTime_;
    static MT_InterpolatedFunction< double > coefSpeedModificator_;
    static const MIL_AgentTypePion* pConvoyAgentType_;
    static const MIL_MissionType_ABC* pConvoyMissionType_;
};

BOOST_CLASS_EXPORT_KEY( PHY_Convoy_ABC )

#endif // __PHY_Convoy_ABC_h_
