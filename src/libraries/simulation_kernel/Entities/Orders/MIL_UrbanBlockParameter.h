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

class DEC_KnowledgeResolver_ABC;
class DEC_Knowledge_Urban;

namespace Common
{
    class MsgUrbanBlock;
}

namespace urban
{
    class Block;
}

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
    explicit MIL_UrbanBlockParameter( const Common::MsgUrbanBlock& asn, const DEC_KnowledgeResolver_ABC& resolver );
    explicit MIL_UrbanBlockParameter( boost::shared_ptr< DEC_Knowledge_Urban > urbanBlock );
    virtual ~MIL_UrbanBlockParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToUrbanBlock( Common::MsgUrbanBlock& ) const;
    virtual bool ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& ) const;
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
    boost::shared_ptr< DEC_Knowledge_Urban > pKnowledgeUrbanBlock_;
    //@}
};

#endif // __MIL_UrbanBlockParameter_h_
