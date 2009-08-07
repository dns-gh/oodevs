// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PlannedWorkParameter_h_
#define __MIL_PlannedWorkParameter_h_

#include "MIL_BaseParameter.h"
class DEC_Gen_Object;
class MIL_EntityManager_ABC;

// =============================================================================
/** @class  MIL_PlannedWorkParameter
    @brief  MIL_PlannedWorkParameter
*/
// Created: LDC 2009-06-04
// =============================================================================
class MIL_PlannedWorkParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_PlannedWorkParameter( const ASN1T_PlannedWork & asn, const MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_PlannedWorkParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToGenObject( ASN1T_PlannedWork& ) const;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PlannedWorkParameter( const MIL_PlannedWorkParameter& );            //!< Copy constructor
    MIL_PlannedWorkParameter& operator=( const MIL_PlannedWorkParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_Gen_Object > pGenObject_;
    //@}
};

#endif // __MIL_PlannedWorkParameter_h_
