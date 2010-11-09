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
#include <boost/shared_ptr.hpp>
#include <boost/serialization/export.hpp>
#include <map>
#include <list>
#include <vector>

class MIL_MedicalTreatmentType;

namespace xml
{
    class xistream;
}

namespace Common
{
    class ObjectAttributeMedicalTreatment;
    class ObjectAttributeMedicalTreatmentBedCapacity;
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
    typedef DEC_Knowledge_ObjectAttributeProxyPassThrough< MedicalTreatmentAttribute > T_KnowledgeProxyType;

public:
    typedef std::pair< float, int >                                      T_PatientDiagnosis; // Patient entry time, Category of injury (UA or UR)
    typedef std::list< T_PatientDiagnosis >                              T_PatientDiagnosisList;
    typedef T_PatientDiagnosisList::const_iterator                     CIT_PatientDiagnosisList;
    typedef T_PatientDiagnosisList::iterator                            IT_PatientDiagnosisList;
    typedef std::map< int, boost::shared_ptr< T_PatientDiagnosisList > > T_MedicalTreatmentMap;
    typedef T_MedicalTreatmentMap::const_iterator                      CIT_MedicalTreatmentMap;
    typedef T_MedicalTreatmentMap::iterator                             IT_MedicalTreatmentMap;

    //! @name Types
    //@{
    class MedicalCapacity
    {
    public:
        typedef std::vector< unsigned int > InjuryCategory; // None, UA, UR, Dead : E_InjuryCategories

        MedicalCapacity() : baseline_( 0 ), occupied_( 4, 0 ), emergency_( 0 ), time_ ( 0 ), type_ ( 0 ) {}

        void        Update( const Common::ObjectAttributeMedicalTreatmentBedCapacity& capacity );
        void        Update( const Common::MsgMissionParameter_Value& capacity );
        void        Send( Common::ObjectAttributeMedicalTreatmentBedCapacity& capacity ) const;
        unsigned    Update( unsigned doctors, float delay );

        //! @name CheckPoints
        //@{ 
        BOOST_SERIALIZATION_SPLIT_MEMBER()
        void load( MIL_CheckPointInArchive&, const unsigned int );
        void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
        //@}

        unsigned        baseline_;
        InjuryCategory  occupied_;
        unsigned        emergency_;
        float           time_;
        const MIL_MedicalTreatmentType* type_;

        bool operator==( const MedicalCapacity& rhs ) const
        {
            return baseline_ == rhs.baseline_ &&
                   occupied_ == rhs.occupied_ &&
                   emergency_ ==  rhs.emergency_ &&
                   time_ == rhs.time_ && 
                   type_ == rhs.type_;

        }

    };

    typedef std::vector< MedicalCapacity > T_TreatmentCapacityVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentAttribute();
    explicit MedicalTreatmentAttribute( xml::xistream& xis );
    explicit MedicalTreatmentAttribute( const Common::MsgMissionParameter_Value& attributes );
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
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void Register( MIL_Object_ABC& object ) const;
    void SendFullState( Common::ObjectAttributes& asn ) const;
    void SendUpdate( Common::ObjectAttributes& asn ) const;
    void OnUpdate( const Common::MsgMissionParameter_Value& attribute );
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    bool Update( const MedicalTreatmentAttribute& rhs );
    //@}

    //! @name Get
    //@{
    int GetDoctors() const;
    int GetAvailableDoctors() const;
    //@}

    //! @name Operators
    //@{
    void RegisterPatients( unsigned injuryID, unsigned category, unsigned n );
    bool CanTreatPatient( unsigned injuryID ) const;
    void Update( float time );
    //@}


private:
    //! @name
    //@{
    void InitializeBedCapacity( xml::xistream& xis );
    void Update( const Common::ObjectAttributeMedicalTreatment& asn );
    //@}

private:
    //! @name Enumerate MsgMissionParameter_Values
    //@{
    enum { eAttributeId, eExternalReferenceId, eDoctors, eStatus, eBedCapacities };
    enum { eTypeId, eBaseLineCount, eAvailableCount, eEmergencyCount };
    //@}

private:
    //! @name Member data
    //@{
    T_MedicalTreatmentMap     medicalTreatmentMap_;
    int                       doctors_;
    int                       availableDoctors_;
    int                       initialDoctors_;
    T_TreatmentCapacityVector capacities_;
    std::string               referenceID_;
    int                       status_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MedicalTreatmentAttribute )

#endif // __MedicalTreatmentAttribute_h_
