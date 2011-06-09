// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LogMaintenancePrioritiesParameter_h_
#define __MIL_LogMaintenancePrioritiesParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_LogMaintenancePrioritiesParameter
    @brief  MIL_LogMaintenancePrioritiesParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_LogMaintenancePrioritiesParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_LogMaintenancePrioritiesParameter();
    explicit MIL_LogMaintenancePrioritiesParameter( const sword::LogMaintenancePriorities & asn );
    virtual ~MIL_LogMaintenancePrioritiesParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const;
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
    T_MaintenancePriorityVector priorities_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LogMaintenancePrioritiesParameter )

#endif // __MIL_LogMaintenancePrioritiesParameter_h_
