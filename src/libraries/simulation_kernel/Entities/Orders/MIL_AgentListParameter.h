// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentListParameter_h_
#define __MIL_AgentListParameter_h_

#include "MIL_BaseParameter.h"

class DEC_Decision_ABC;
class MIL_EntityManager_ABC;

// =============================================================================
/** @class  MIL_AgentListParameter
    @brief  MIL_AgentListParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AgentListParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentListParameter( const Common::UnitIdList& asn, MIL_EntityManager_ABC& entityManager );
    explicit MIL_AgentListParameter( const std::vector< DEC_Decision_ABC* >& unitList );
    virtual ~MIL_AgentListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToAgentList( Common::UnitIdList& asn ) const;
    virtual bool ToAgentList( std::vector< DEC_Decision_ABC* >& value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AgentListParameter( const MIL_AgentListParameter& );            //!< Copy constructor
    MIL_AgentListParameter& operator=( const MIL_AgentListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< DEC_Decision_ABC* > unitList_;
    //@}
};

#endif // __MIL_AgentListParameter_h_
