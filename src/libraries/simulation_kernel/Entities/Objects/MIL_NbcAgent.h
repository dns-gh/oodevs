// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_NbcAgent_h_
#define __MIL_NbcAgent_h_

#include "MIL.h"

class MIL_NbcAgentType;

// =============================================================================
/** @class  MIL_NbcAgent
    @brief  MIL_NbcAgent
*/
// Created: NLD 2006-10-27
// =============================================================================
class MIL_NbcAgent
{
    MT_COPYNOTALLOWED( MIL_NbcAgent )

public:
    //! @name Types
    //@{
    enum E_Form
    {
        eLiquid,
        eGas
    };
    //@}

public:
     MIL_NbcAgent();
     MIL_NbcAgent( const MIL_NbcAgentType& type, E_Form nForm );
    ~MIL_NbcAgent();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Accessors
    //@{
    bool                    IsPoisonous        () const;
    bool                    IsContaminating    () const;
    const PHY_HumanWound&   GetRandomWound     () const; 
    uint                    GetLifeTime        () const;
    MT_Float                GetPropagationAngle() const;
    bool                    CanBeVaporized     () const;
    const MIL_NbcAgentType& GetType            () const;
    //@}

private:
    const MIL_NbcAgentType* pType_;
    const E_Form            nForm_;
};

#include "MIL_NbcAgent.inl"

#endif // __MIL_NbcAgent_h_
