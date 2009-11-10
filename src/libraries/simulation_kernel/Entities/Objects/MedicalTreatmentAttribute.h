// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MedicalTreatmentAttribute_h_
#define __MedicalTreatmentAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/shared_ptr.hpp>
#include "MIL_MedicalTreatmentType.h"

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: RFT 2008-05-30
// =============================================================================
class MedicalTreatmentAttribute 
    : public ObjectAttribute_ABC
	, public UpdatableAttribute_ABC
{ 
public:
    typedef std::pair< float , int >                                             T_PatientDiagnosis; //Patient entry time, Category of injury (UA or UR)
    typedef std::list< T_PatientDiagnosis >                                      T_PatientDiagnosisList;
    typedef T_PatientDiagnosisList::const_iterator                               CIT_PatientDiagnosisList;
    typedef T_PatientDiagnosisList::iterator                                     IT_PatientDiagnosisList;
    typedef std::map< int, boost::shared_ptr< T_PatientDiagnosisList > > T_MedicalTreatmentMap;
    typedef T_MedicalTreatmentMap::const_iterator                                CIT_MedicalTreatmentMap;
    typedef T_MedicalTreatmentMap::iterator                                      IT_MedicalTreatmentMap;
    

public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentAttribute();    
    explicit MedicalTreatmentAttribute( xml::xistream& xis );
    explicit MedicalTreatmentAttribute( const ASN1T_ObjectAttributes& asn );
    virtual ~MedicalTreatmentAttribute();
    //@}

private:
    //! @name Initialize
    //@{
    void InitializePatientDiagnosisList( int occupiedBeds , int occupiedDoctors );
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name 
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    //@}

    //! @name Get
    //@{
    const T_MedicalTreatmentMap& GetMap() const;
    int GetBeds() const;
    int GetAvailableBeds() const;
    int GetDoctors() const;
    int GetAvailableDoctors() const;
    //@}

    //! @name Operators
    //@{
    void FlagPatient( float time , int injuryID , int injuryCategory );
    void FreeDoctors( float time );
    void FreeBeds( float time );
    bool CanTreatPatient( int injuryID );
    //@}

    //! @name Update Functions
    //@{
    void UpdateAvailableBeds( bool bEmergencyPlan , float emergencyBedsRate );
    void UpdateAvailableDoctors( bool bEmergencyPlan , bool bBusinessHours , float emergencyDoctorsRate , float nightDoctorsRate );
    //@}

private:
    //! @name Member data
    //@{
    static MT_Random      randomGenerator_;
    T_MedicalTreatmentMap medicalTreatmentMap_;
    int                   beds_;            
    int                   availableBeds_;   
    int                   doctors_;         
    int                   availableDoctors_;
    int                   initialBeds_;
    int                   initialDoctors_;
    //@}
};

#endif // __MedicalTreatmentAttribute_h_
