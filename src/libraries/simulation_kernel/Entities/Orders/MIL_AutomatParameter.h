// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AutomatParameter_h_
#define __MIL_AutomatParameter_h_

#include "MIL_BaseParameter.h"
class DEC_AutomateDecision;
class MIL_EntityManager_ABC;

// =============================================================================
/** @class  MIL_AutomatParameter
    @brief  MIL_AutomatParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AutomatParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_AutomatParameter( DEC_AutomateDecision* pDecision );
             MIL_AutomatParameter( const ASN1T_Automat&, MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_AutomatParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToAutomat( ASN1T_Unit& ) const;
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AutomatParameter( const MIL_AutomatParameter& );            //!< Copy constructor
    MIL_AutomatParameter& operator=( const MIL_AutomatParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    DEC_AutomateDecision* pDecision_;
    //@}
};

#endif // __MIL_AutomatParameter_h_
