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

#include "ObjectAttribute_ABC.h"
#include <list>

namespace dispatcher
{

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: RFT 2006-09-19
// =============================================================================
class MedicalTreatmentAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalTreatmentAttribute( const Common::ObjectAttributes& asnMsg );
    virtual ~MedicalTreatmentAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::ObjectAttributes& asnMsg );
    virtual void Send  ( Common::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    int              beds_;
    int              availableBeds_;
    int              doctors_;
    int              availableDoctors_;
    std::list< int > medicalTreatmentList_; // XML reference - no resolved by dispatcher
    //@}
};

}

#endif // __MedicalTreatmentAttribute_h_
