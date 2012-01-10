// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalCollectionAmbulance.h $
// $Author: Jvt $
// $Modtime: 11/04/05 14:34 $
// $Revision: 2 $
// $Workfile: PHY_MedicalCollectionAmbulance.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalCollectionAmbulance_h_
#define __PHY_MedicalCollectionAmbulance_h_

#include <boost/serialization/export.hpp>

class PHY_ComposantePion;
class PHY_MedicalCollectionConsign;
class PHY_RoleInterface_Medical;

// =============================================================================
// @class  PHY_MedicalCollectionAmbulance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalCollectionAmbulance : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PHY_MedicalCollectionAmbulance( PHY_RoleInterface_Medical& medical, PHY_ComposantePion& compAmbulance );
     PHY_MedicalCollectionAmbulance();
    virtual ~PHY_MedicalCollectionAmbulance();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    bool Update();
    void Cancel();

    bool RegisterHuman  ( PHY_MedicalCollectionConsign& consign );
    void UnregisterHuman( PHY_MedicalCollectionConsign& consign );
    //@}

    //! @name Accessors
    //@{
    bool IsAnEmergency() const;
    unsigned int GetNbrHumans () const;
    int          GetTimer() const;
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
    //! @name Member data
    //@{
    PHY_RoleInterface_Medical*  pMedical_;
    PHY_ComposantePion*         pCompAmbulance_;
    T_ConsignVector             consigns_;
    E_State                     nState_;
    int                         nTimer_;
    bool                        bEmergencyAmbulance_;
    double                    rNbrHumanHandled_;
    PHY_RoleInterface_Medical*  pSortingArea_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MedicalCollectionAmbulance )

#endif // __PHY_MedicalCollectionAmbulance_h_
