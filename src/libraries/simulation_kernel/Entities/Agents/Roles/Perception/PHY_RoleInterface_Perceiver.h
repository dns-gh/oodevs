// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:25 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Perceiver.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Perceiver_h_
#define __PHY_RoleInterface_Perceiver_h_

#include "MT_Tools/Role_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionSurfaceAgent.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionSurfaceObject.h"

namespace client
{
    class UnitAttributes;
}

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class PHY_PerceptionLevel;
class PHY_RadarClass;
class PHY_RadarType;
class MIL_Effect_IndirectFire;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;
class TER_Localisation;
class UrbanObjectWrapper;

// =============================================================================
// @class  PHY_RoleInterface_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RoleInterface_Perceiver : public tools::Role_ABC
                                  , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Perceiver RoleInterface;

    typedef std::pair< const PHY_SensorTypeAgent*, double /*height*/ >  T_SurfaceAgentKeyPair;
    typedef std::map< T_SurfaceAgentKeyPair, PHY_PerceptionSurfaceAgent > T_SurfaceAgentMap;
    typedef T_SurfaceAgentMap::const_iterator                           CIT_SurfaceAgentMap;
    typedef T_SurfaceAgentMap::const_iterator                            IT_SurfaceAgentMap;

    typedef std::pair< const PHY_SensorTypeObject* , double /*height*/ >  T_SurfaceObjectKeyPair;
    typedef std::map< T_SurfaceObjectKeyPair, PHY_PerceptionSurfaceObject > T_SurfaceObjectMap;
    typedef T_SurfaceObjectMap::const_iterator                            CIT_SurfaceObjectMap;

    typedef std::set< const PHY_RadarType* > T_RadarSet;
    typedef T_RadarSet::const_iterator     CIT_RadarSet;

    typedef std::map< const PHY_RadarClass*, T_RadarSet > T_RadarsPerClassMap;
    typedef T_RadarsPerClassMap::const_iterator         CIT_RadarsPerClassMap;
    //@}
    //@}

public:
             PHY_RoleInterface_Perceiver() {}
    virtual ~PHY_RoleInterface_Perceiver() {}

    //! @name Operations
    //@{
    virtual bool IsUsingActiveRadar() const = 0;
    virtual bool IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const = 0; // LTO
    //@}

    virtual void NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded ) = 0;
    virtual bool NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( MIL_Object_ABC& object, const PHY_PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape ) = 0;
    virtual void NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const = 0;
    virtual void NotifyPerception( const UrbanObjectWrapper& block, const PHY_PerceptionLevel& level ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool HasChanged() const = 0;
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    virtual void ExecutePerceptions() = 0;
    virtual const PHY_PerceptionLevel& ComputePerception( const MT_Vector2D& vPoint ) const = 0;
    virtual const PHY_PerceptionLevel& ComputePerception( const DEC_Knowledge_Object& knowledge ) const = 0;
    virtual const PHY_PerceptionLevel& ComputePerception( const DEC_Knowledge_Agent & knowledge ) const = 0;

    virtual void SetVisionModeNormal() = 0;;
    virtual void SetVisionModeDirection( const MT_Vector2D& vDirection ) = 0;
    virtual void SetVisionModePoint( const MT_Vector2D& vPoint ) = 0;

    virtual bool HasRadarStateChanged() const = 0;
    //@}

    //! @name Perceptions
    //@{
    virtual void EnableSensors() = 0;
    virtual void DisableSensors() = 0;

    virtual bool HasDelayedPerceptions() const = 0;
    virtual void EnableRecoAlat( const TER_Localisation& localisation ) = 0;
    virtual void DisableRecoAlat() = 0;
    virtual void EnableCoupDeSonde() = 0;
    virtual void DisableCoupDeSonde() = 0;
    virtual int EnableRecoPoint( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent ) = 0;
    virtual void DisableRecoPoint( int ) = 0;
    virtual int EnableRecoLocalisation( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent ) = 0;
    virtual int EnableRecoLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent ) = 0;
    virtual int EnableRecoUrbanBlock( UrbanObjectWrapper* pUrbanBlock ) = 0;
    virtual void DisableRecoUrbanBlock( int ) = 0;
    virtual int EnableControlLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent ) = 0;
    virtual void DisableRecoLocalisation( int ) = 0;
    virtual int EnableSurveillanceLocalisation( const TER_Localisation& localisation ) = 0;
    virtual void DisableSurveillanceLocalisation( int ) = 0;
    virtual int EnableRecoObjects( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent ) = 0;
    virtual void DisableRecoObjects( int ) = 0;

    // Radars
    virtual int EnableRadarOnLocalisation( const PHY_RadarClass& radarClass, const TER_Localisation& localisation ) = 0;
    virtual void DisableRadarOnLocalisation( const PHY_RadarClass& radarClass, int ) = 0;
    virtual void EnableRadar( const PHY_RadarClass& radarClass ) = 0;
    virtual void DisableRadar( const PHY_RadarClass& radarClass ) = 0;

    virtual int EnableFlyingShellDetection( const TER_Localisation& localisation ) = 0;
    virtual void DisableFlyingShellDetection( int ) = 0;

    // Rens
    virtual void EnableRecordMode () = 0;
    virtual void DisableRecordMode() = 0;

    virtual void DisableAllPerceptions() = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const T_SurfaceAgentMap& GetSurfacesAgent () const = 0;
    virtual const T_SurfaceObjectMap& GetSurfacesObject() const = 0;
    virtual const T_RadarSet& GetRadars( const PHY_RadarClass& radarClass ) = 0;
    //@}

    //! @name Tools
    //@{
    virtual const MIL_KnowledgeGroup& GetKnowledgeGroup() const = 0;
    virtual MIL_Agent_ABC& GetPion() const = 0;
    virtual double GetMaxAgentPerceptionDistance() const = 0;
    virtual void GetMainPerceptionDirection( MT_Vector2D& vDirection ) const = 0;

    virtual bool IsPeriphericalVisionEnabled() const = 0;
    virtual bool IsKnown( const MIL_Agent_ABC& agent ) const = 0;
    virtual bool IsIdentified( const MIL_Agent_ABC& agent ) const = 0;
    virtual bool IsKnown( const MIL_Object_ABC& object ) const = 0;
    virtual bool IsIdentified( const MIL_Object_ABC& object ) const = 0;
    virtual bool IsIdentified( const MIL_PopulationConcentration& concentration ) const = 0;
    virtual bool IsIdentified( const UrbanObjectWrapper& object ) const = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendDebugState() const = 0;
    virtual void SendChangedState( client::UnitAttributes& msg ) const = 0;
    virtual void SendFullState( client::UnitAttributes& msg ) const = 0;
    //@}
private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

//} // namespace perception
//} // namespace simulation
#endif // __PHY_RoleInterface_Perceiver_h_
