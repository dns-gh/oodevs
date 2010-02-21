 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAttribute_h_
#define __NBCAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "protocol/SimulationSenders.h"

namespace Common
{
    class MsgObjectAttributes;
    class MsgObjectAttributeNBC;
}

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  NBCAttribute
    @brief  NBCAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class NBCAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     NBCAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg );
    virtual ~NBCAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send     ( Common::MsgObjectAttributes& asnMsg ) const;
    virtual void Delete( Common::MsgObjectAttributes& asnMsg ) const;
    //@}
private:
    //! @name 
    //@{
    void Clear();
    //@}
private:
    Common::MsgObjectAttributeNBC nbc_; // XML reference - no resolved by dispatcher
};

}

#endif // __NBCAttribute_h_
