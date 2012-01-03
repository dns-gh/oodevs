// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalEvacuationAmbulance.h $
// $Author: Jvt $
// $Modtime: 11/04/05 14:30 $
// $Revision: 2 $
// $Workfile: PHY_MedicalEvacuationAmbulance.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalEvacuationAmbulance_h_
#define __PHY_MedicalEvacuationAmbulance_h_

#include <boost/serialization/export.hpp>

class PHY_ComposantePion;
class PHY_MedicalEvacuationConsign;
class PHY_RoleInterface_Medical;

// =============================================================================
// @class  PHY_MedicalEvacuationAmbulance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalEvacuationAmbulance : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PHY_MedicalEvacuationAmbulance( PHY_RoleInterface_Medical& medical, PHY_ComposantePion& compAmbulance );
     PHY_MedicalEvacuationAmbulance();
    virtual ~PHY_MedicalEvacuationAmbulance();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    bool Update();

    bool RegisterHuman  ( PHY_MedicalEvacuationConsign& consign );
    void UnregisterHuman( PHY_MedicalEvacuationConsign& consign );
    //@}

    //! @name Accessors
    //@{
    int GetTimer() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_MedicalEvacuationConsign* > T_ConsignVector;
    typedef T_ConsignVector::iterator                    IT_ConsignVector;
    typedef T_ConsignVector::const_iterator              CIT_ConsignVector;

    enum E_State
    {
        eWaiting,
        eGoingTo,
        eLoading,
        eGoingFrom,
        eUnloading,
        eFinished
    };
    //@}

private:
    //! @name Tools
    //@{
    void EnterStateGoingTo  ();
    void EnterStateLoading  ();
    void EnterStateGoingFrom();
    void EnterStateUnloading();
    void EnterStateFinished ();

    bool DoLoading  ();
    bool DoUnloading();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleInterface_Medical*  pMedical_;
    PHY_ComposantePion*         pCompAmbulance_;
    T_ConsignVector             consigns_;
    E_State                     nState_;
    int                         nTimer_;
    double                      rNbrHumanHandled_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MedicalEvacuationAmbulance )

#endif // __PHY_MedicalEvacuationAmbulance_h_
