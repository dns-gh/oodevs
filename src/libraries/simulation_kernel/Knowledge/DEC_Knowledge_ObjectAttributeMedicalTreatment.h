// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectMedicalTreatment.h $
// $Author: RFT $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectMedicalTreatment.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeMedicalTreatment_h_
#define __DEC_Knowledge_ObjectAttributeMedicalTreatment_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class MedicalTreatmentAttribute;
class MIL_MedicalTreatmentType;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeMedicalTreatment : public DEC_Knowledge_ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeMedicalTreatment();
    explicit DEC_Knowledge_ObjectAttributeMedicalTreatment( const MedicalTreatmentAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeMedicalTreatment();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void Register( DEC_Knowledge_Object& knObject );
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( Common::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< int >                             T_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::iterator        IT_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::const_iterator CIT_MedicalTreatmentTypeList;
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes();
    //@}

private:
    //! @name Data Members
    //@{
    const MedicalTreatmentAttribute* attr_;
    T_MedicalTreatmentTypeList medicalTreatmentList_;
    int availableBeds_;
    int availableDoctors_;
    int beds_;
    int doctors_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeMedicalTreatment )

#endif // __DEC_Knowledge_ObjectAttributeMedicalTreatment_h_
