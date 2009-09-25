// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AutomatListParameter_h_
#define __MIL_AutomatListParameter_h_

#include "MIL_BaseParameter.h"
class DEC_AutomateDecision;
class MIL_EntityManager_ABC;

// =============================================================================
/** @class  MIL_AutomatListParameter
    @brief  MIL_AutomatListParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AutomatListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_AutomatListParameter( const ASN1T_AutomatList&, MIL_EntityManager_ABC& entityManager );
    explicit MIL_AutomatListParameter( const std::vector< DEC_AutomateDecision* >& automatList );
    virtual ~MIL_AutomatListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToAutomatList( ASN1T_AutomatList& ) const;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AutomatListParameter( const MIL_AutomatListParameter& );            //!< Copy constructor
    MIL_AutomatListParameter& operator=( const MIL_AutomatListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< DEC_AutomateDecision* > automatList_;
    //@}
};

#endif // __MIL_AutomatListParameter_h_
