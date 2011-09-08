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

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"

class MIL_MedicalTreatmentType;

namespace xml
{
    class xistream;
}

namespace sword
{
    class ObjectAttributeMedicalTreatment;
    class MedicalTreatmentBedCapacity;
}

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
    //! @name Types
    //@{
    typedef DEC_Knowledge_ObjectAttributeProxyPassThrough< MedicalTreatmentAttribute > T_KnowledgeProxyType;
    typedef std::pair< float, int >                                      T_PatientDiagnosis; // Patient entry time, Category of injury (UA or UR)
    typedef std::list< T_PatientDiagnosis >                              T_PatientDiagnosisList;
    typedef T_PatientDiagnosisList::iterator                            IT_PatientDiagnosisList;
    typedef T_PatientDiagnosisList::const_iterator                     CIT_PatientDiagnosisList;
    typedef std::map< int, boost::shared_ptr< T_PatientDiagnosisList > > T_MedicalTreatmentMap;
    typedef T_MedicalTreatmentMap::iterator                             IT_MedicalTreatmentMap;
    typedef T_MedicalTreatmentMap::const_iterator                      CIT_MedicalTreatmentMap;

    class MedicalCapacity
    {
    public:
        //! @name Types
        //@{
        typedef std::vector< unsigned int > InjuryCategory; // None, UA, UR, Dead : E_InjuryCategories
        //@}

        //! @name Construtor
        //@{
        MedicalCapacity()
            : baseline_ ( 0 )
            , occupied_ ( 4, 0 )
            , emergency_( 0 )
            , time_     ( 0 )
            , type_     ( 0 )
        {
            // NOTHING
        }
        //@}

        //! @name Operations
        //@{
        void Update( const sword::MedicalTreatmentBedCapacity& capacity );
        void Update( const sword::MissionParameter_Value& capacity );
        void Send( sword::MedicalTreatmentBedCapacity& capacity ) const;
        unsigned int Update( unsigned int doctors, float delay );
        bool operator==( const MedicalCapacity& rhs ) const
        {
            return baseline_ == rhs.baseline_ &&
                   occupied_ == rhs.occupied_ &&
                   emergency_ ==  rhs.emergency_ &&
                   time_ == rhs.time_ &&
                   type_ == rhs.type_;
        }
        //@}

        //! @name CheckPoints
        //@{
        BOOST_SERIALIZATION_SPLIT_MEMBER()
        void load( MIL_CheckPointInArchive&, const unsigned int );
        void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
        //@}

        //! @name Member data
        //@{
        unsigned int baseline_;
        InjuryCategory occupied_;
        unsigned int emergency_;
        float time_;
        const MIL_MedicalTreatmentType* type_;
        //@}
    };
    typedef std::map< int, MedicalCapacity > T_TreatmentCapacityVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentAttribute();
    explicit MedicalTreatmentAttribute( xml::xistream& xis );
    explicit MedicalTreatmentAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~MedicalTreatmentAttribute();
    //@}

private:
    //! @name Initialize
    //@{
    void InitializePatientDiagnosisList( int occupiedBeds, int occupiedDoctors );
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void Register( MIL_Object_ABC& object ) const;
    void SendFullState( sword::ObjectAttributes& asn ) const;
    bool SendUpdate( sword::ObjectAttributes& asn ) const;
    void OnUpdate( const sword::MissionParameter_Value& attribute );
    void WriteODB( xml::xostream& xos ) const;
    void RegisterPatients( unsigned int injuryID, unsigned int category, int n );
    bool CanTreatPatient( unsigned int injuryID ) const;
    void Update( float time );
    //@}

    //! @name Operations
    //@{
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    bool Update( const MedicalTreatmentAttribute& rhs );
    //@}

    //! @name Accessors
    //@{
    int GetDoctors() const;
    int GetAvailableDoctors() const;
    //@}

private:
    //! @name
    //@{
    void InitializeBedCapacity( xml::xistream& xis );
    void Update( const sword::ObjectAttributeMedicalTreatment& asn );
    //@}

private:
    //! @name Enumerate MissionParameter_Values
    //@{
    enum { eAttributeId, eExternalReferenceId, eDoctors, eStatus, eBedCapacities };
    enum { eTypeId, eBaseLineCount, eAvailableCount, eEmergencyCount };
    //@}

private:
    //! @name Member data
    //@{
    T_MedicalTreatmentMap medicalTreatmentMap_;
    int doctors_;
    int availableDoctors_;
    int initialDoctors_;
    T_TreatmentCapacityVector capacities_;
    std::string referenceID_;
    int status_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MedicalTreatmentAttribute )

#endif // __MedicalTreatmentAttribute_h_
