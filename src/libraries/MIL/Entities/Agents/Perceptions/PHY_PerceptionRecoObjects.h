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

#include "PHY_Perception_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
// @class  PHY_PerceptionRecoObjects
// Created: JVT 2004-10-21
// =============================================================================
class PHY_PerceptionRecoObjects : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRecoObjects )

public:
             PHY_PerceptionRecoObjects( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoObjects();

    void Update();

    //! @name Add/Remove Points
    //@{
    void* AddLocalisation        ( const TER_Localisation& localisation, const MT_Vector2D& vCenter, MT_Float rSpeed, DIA_Variable_ABC& result );
    void  RemoveLocalisation     ( void* );

    bool  HasLocalisationToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_RealObject_ABC&   object    ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sReco
    {
        sReco( const TER_Localisation& localisation, const MT_Vector2D& vCenter, MT_Float rGrowthSpeed, DIA_Variable_ABC& result );
        
        bool IsInside        ( const TER_Localisation& )                 const;
        void GetObjectsInside( TER_Object_ABC::T_ObjectVector& ) const;

        void UpdateLocalisation();

    private:
        sReco& operator = ( const sReco& );
        
    private:
        const MT_Vector2D      vCenter_;
        const TER_Localisation localisation_;
              TER_Localisation circle_;
        MT_Float               rCurrentSize_;
        const MT_Float         rGrowthSpeed_;
        DIA_Variable_ABC*      pReturn_;
        bool                   bMaxSizeDone_;
    };    

    typedef std::vector< sReco* >        T_RecoVector;
    typedef T_RecoVector::iterator       IT_RecoVector;
    typedef T_RecoVector::const_iterator CIT_RecoVector;
    //@}

private:
    T_RecoVector recos_;
};

#include "PHY_PerceptionRecoObjects.inl"

#endif // __PHY_PerceptionRecoObjects_h_
