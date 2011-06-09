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
             MIL_AutomatParameter();
    explicit MIL_AutomatParameter( DEC_AutomateDecision* pDecision );
             MIL_AutomatParameter( const sword::AutomatId&, MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_AutomatParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    DEC_AutomateDecision* pDecision_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AutomatParameter )

#endif // __MIL_AutomatParameter_h_
