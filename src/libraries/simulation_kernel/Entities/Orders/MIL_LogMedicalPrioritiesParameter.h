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
    virtual bool ToElement( sword::MsgMissionParameter_Value& elem ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_LogMedicalPrioritiesParameter( const MIL_LogMedicalPrioritiesParameter& );            //!< Copy constructor
    MIL_LogMedicalPrioritiesParameter& operator=( const MIL_LogMedicalPrioritiesParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_MedicalPriorityVector priorities_;
    //@}
};

#endif // __MIL_LogMedicalPrioritiesParameter_h_
