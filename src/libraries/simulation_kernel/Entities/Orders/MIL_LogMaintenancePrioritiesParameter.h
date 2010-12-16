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

private:
    //! @name Copy/Assignment
    //@{
    MIL_LogMaintenancePrioritiesParameter( const MIL_LogMaintenancePrioritiesParameter& );            //!< Copy constructor
    MIL_LogMaintenancePrioritiesParameter& operator=( const MIL_LogMaintenancePrioritiesParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_MaintenancePriorityVector priorities_;
    //@}
};

#endif // __MIL_LogMaintenancePrioritiesParameter_h_
