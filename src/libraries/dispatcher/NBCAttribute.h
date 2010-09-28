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

namespace Common
{
    class ObjectAttributeNBC;
}

namespace dispatcher
{

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
    explicit NBCAttribute( const Common::ObjectAttributes& asnMsg );
    virtual ~NBCAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::ObjectAttributes& asnMsg );
    virtual void Send( Common::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int danger_;
    std::vector< unsigned int > agents_;
    //Common::ObjectAttributeNBC nbc_; // XML reference - no resolved by dispatcher
    //@
};

}

#endif // __NBCAttribute_h_
