// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanBlockParameter_h_
#define __MIL_UrbanBlockParameter_h_

#include "MIL_BaseParameter.h"

namespace urban
{
    class Block;
}

class MIL_AgentServer;

// =============================================================================
/** @class  MIL_UrbanBlockParameter
    @brief  MIL_UrbanBlockParameter
*/
// Created: MGD 2009-11-02
// =============================================================================
class MIL_UrbanBlockParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_UrbanBlockParameter( const ASN1T_UrbanBlock& asn, MIL_AgentServer& entityManager );
    virtual ~MIL_UrbanBlockParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToUrbanBlock( ASN1T_UrbanBlock& ) const;
    virtual bool ToUrbanBlock( urban::Block*& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_UrbanBlockParameter( const MIL_UrbanBlockParameter& );            //!< Copy constructor
    MIL_UrbanBlockParameter& operator=( const MIL_UrbanBlockParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    urban::Block* pUrbanBlock_;
    //@}
};

#endif // __MIL_UrbanBlockParameter_h_
