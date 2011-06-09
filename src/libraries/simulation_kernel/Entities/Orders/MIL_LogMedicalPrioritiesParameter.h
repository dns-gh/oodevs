// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LogMedicalPrioritiesParameter_h_
#define __MIL_LogMedicalPrioritiesParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_LogMedicalPrioritiesParameter
    @brief  MIL_LogMedicalPrioritiesParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_LogMedicalPrioritiesParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_LogMedicalPrioritiesParameter();
    explicit MIL_LogMedicalPrioritiesParameter( const sword::LogMedicalPriorities& asn );
    virtual ~MIL_LogMedicalPrioritiesParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_MedicalPriorityVector priorities_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LogMedicalPrioritiesParameter )

#endif // __MIL_LogMedicalPrioritiesParameter_h_
