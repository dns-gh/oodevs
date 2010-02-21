 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionAttribute_h_
#define __ConstructionAttribute_h_


#include "ObjectAttribute_ABC.h"


namespace Common
{
    class MsgObjectAttributes;
}


namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ConstructionAttribute
    @brief  ConstructionAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ConstructionAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ConstructionAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg );
    virtual ~ConstructionAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send     ( Common::MsgObjectAttributes& asnMsg ) const;
    virtual void Delete( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    unsigned     dotation_;
    unsigned int nPercentageConstruction_;
    unsigned int nNbrDotationForConstruction_;
    //@}
    
};

}

#endif // __ConstructionAttribute_h_
