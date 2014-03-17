// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoObjects.h $
// $Author: Nld $
// $Modtime: 2/05/05 15:43 $
// $Revision: 4 $
// $Workfile: PHY_PerceptionRecoObjects.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRecoObjects_h_
#define __PHY_PerceptionRecoObjects_h_

#include "PHY_PerceptionWithLocation.h"
#include "PHY_PerceptionLocalisation.h"
#include "simulation_terrain/TER_Localisation.h"

class DEC_Decision_ABC;

class PHY_PerceptionRecoObjectsReco : public PHY_PerceptionLocalisation
{
public:
    PHY_PerceptionRecoObjectsReco( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rGrowthSpeed, DEC_Decision_ABC& callerAgent );

    bool IsInside        ( const TER_Localisation& )                 const;
    void GetObjectsInside( TER_Object_ABC::T_ObjectVector& ) const;

    void UpdateLocalisation();

private:
    PHY_PerceptionRecoObjectsReco& operator = ( const PHY_PerceptionRecoObjectsReco& );

private:
    const MT_Vector2D      vCenter_;
    const TER_Localisation localisation_;
          TER_Localisation circle_;
    double                 rCurrentSize_;
    const double           rGrowthSpeed_;
    DEC_Decision_ABC&      callerAgent_;
    bool                   bMaxSizeDone_;
};

// =============================================================================
// @class  PHY_PerceptionRecoObjects
// Created: JVT 2004-10-21
// =============================================================================
class PHY_PerceptionRecoObjects : public PHY_PerceptionWithLocation< PHY_PerceptionRecoObjectsReco >
{
public:
    explicit PHY_PerceptionRecoObjects( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoObjects();

    void Update();

    //! @name Add/Remove Points
    //@{
    int   AddLocalisation        ( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent );
    void  RemoveLocalisation     ( int id );

    bool  HasLocalisationToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    //@}
};

#endif // __PHY_PerceptionRecoObjects_h_
