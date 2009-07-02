// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalCollectionAmbulance.h $
// $Author: Jvt $
// $Modtime: 11/04/05 14:34 $
// $Revision: 2 $
// $Workfile: PHY_MedicalCollectionAmbulance.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalCollectionAmbulance_h_
#define __PHY_MedicalCollectionAmbulance_h_

class PHY_ComposantePion;
class PHY_MedicalCollectionConsign;
class PHY_RolePionLOG_Medical;
class PHY_RolePion_Medical;

// =============================================================================
// @class  PHY_MedicalCollectionAmbulance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalCollectionAmbulance : private boost::noncopyable
{

public:
     PHY_MedicalCollectionAmbulance( PHY_RolePionLOG_Medical& medical, PHY_ComposantePion& compAmbulance );
     PHY_MedicalCollectionAmbulance();
    ~PHY_MedicalCollectionAmbulance();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    bool Update();

    bool RegisterHuman  ( PHY_MedicalCollectionConsign& consign );
    void UnregisterHuman( PHY_MedicalCollectionConsign& consign );
    //@}

    //! @name Accessors
    //@{
    bool IsAnEmergency() const;
    uint GetNbrHumans () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_MedicalCollectionConsign* > T_ConsignVector;
    typedef T_ConsignVector::iterator                    IT_ConsignVector;
    typedef T_ConsignVector::const_iterator              CIT_ConsignVector;

    enum E_State
    {
        eWaiting,
        eGoingTo,
        eLoading,
        eSearchingForSortingArea,
        eGoingFrom,
        eUnloading,
        eFinished
    };
    //@}

private:
    //! @name Tools
    //@{
    void EnterStateLoading                ();
    void EnterStateSearchingForSortingArea();
    void ChooseStateFromSortingAreaState  ();
    void EnterStateGoingTo                ();
    void EnterStateUnloading              ();
    void EnterStateGoingFrom              ();
    void EnterStateFinished               ();
    
    bool DoLoading             ();
    bool DoUnloading           ();
    bool DoSearchForSortingArea();
    //@}

private:
    PHY_RolePionLOG_Medical* pMedical_;
    PHY_ComposantePion*      pCompAmbulance_;
    T_ConsignVector          consigns_;
    E_State                  nState_;
    int                      nTimer_;
    bool                     bEmergencyAmbulance_;
    MT_Float                 rNbrHumanHandled_;
    PHY_RolePion_Medical*    pSortingArea_;
};

#endif // __PHY_MedicalCollectionAmbulance_h_
