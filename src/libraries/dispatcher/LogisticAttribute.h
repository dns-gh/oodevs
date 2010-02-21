 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_


#include "ObjectAttribute_ABC.h"


namespace Common
{
    class MsgObjectAttributes;
}


namespace dispatcher
{
    class Model;
    class Automat;

// =============================================================================
/** @class  LogisticAttribute
    @brief  LogisticAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogisticAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     LogisticAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg );
    virtual ~LogisticAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send     ( Common::MsgObjectAttributes& asnMsg ) const;
    virtual void Delete( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    const Model&   model_;
    const Automat* pTC2_;
};

}

#endif // __LogisticAttribute_h_
