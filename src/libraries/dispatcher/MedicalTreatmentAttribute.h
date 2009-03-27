 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentAttribute_h_
#define __MedicalTreatmentAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"
#include <list>

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: RFT 2006-09-19
// =============================================================================
class MedicalTreatmentAttribute : public ObjectAttribute_ABC
{
private:
    typedef std::list< int >                           T_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::const_iterator CIT_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::iterator       IT_MedicalTreatmentTypeList;

public:
    //! @name Constructors/Destructor
    //@{
     MedicalTreatmentAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~MedicalTreatmentAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    int                        beds_;            
    int                        availableBeds_;   
    int                        doctors_;         
    int                        availableDoctors_;
    T_MedicalTreatmentTypeList medicalTreatmentList_; // XML reference - no resolved by dispatcher
    //@}
};

}

#endif // __MedicalTreatmentAttribute_h_
