// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_MissionObjectiveListParameter_h_
#define __MIL_MissionObjectiveListParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_MissionObjectiveListParameter
    @brief  MIL_MissionObjectiveListParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_MissionObjectiveListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_MissionObjectiveListParameter( const ASN1T_MissionObjectiveList& asn );
    virtual ~MIL_MissionObjectiveListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_MissionObjectiveListParameter( const MIL_MissionObjectiveListParameter& );            //!< Copy constructor
    MIL_MissionObjectiveListParameter& operator=( const MIL_MissionObjectiveListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< DEC_Objective > > objectives_;
    //@}
};

#endif // __MIL_MissionObjectiveListParameter_h_
