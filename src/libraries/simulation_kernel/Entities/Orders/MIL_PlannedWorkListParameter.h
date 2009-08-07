// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PlannedWorkListParameter_h_
#define __MIL_PlannedWorkListParameter_h_

#include "MIL_BaseParameter.h"
#include "simulation_terrain/TER_Localisation.h"
class DEC_Gen_Object;
class MIL_EntityManager_ABC;

// =============================================================================
/** @class  MIL_PlannedWorkListParameter
    @brief  MIL_PlannedWorkListParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_PlannedWorkListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_PlannedWorkListParameter( const ASN1T_PlannedWorkList&, const MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_PlannedWorkListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToGenObjectList( ASN1T_PlannedWorkList& ) const;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PlannedWorkListParameter( const MIL_PlannedWorkListParameter& );            //!< Copy constructor
    MIL_PlannedWorkListParameter& operator=( const MIL_PlannedWorkListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > plannedWorkList_;
    //@}
};

#endif // __MIL_PlannedWorkListParameter_h_
