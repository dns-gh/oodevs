// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VegetationAttribute_h_
#define __VegetationAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace MsgsSimToClient
{
    class MsgUrbanAttributes;
}

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  VegetationAttribute
@brief  VegetationAttribute
*/
// Created: SLG 2009-12-04
// =============================================================================
class VegetationAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    VegetationAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message  );
    virtual ~VegetationAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const MsgsSimToClient::MsgUrbanAttributes& message  );
    virtual void Send     (       MsgsSimToClient::MsgUrbanAttributes& message  ) const;
    virtual void AsnDelete(       MsgsSimToClient::MsgUrbanAttributes& message  ) const;
    //@}

private:
    //! @name Data members
    //@{
    std::string     type_;
    unsigned int    height_;
    float           density_;
    //@}
};

}

#endif // __VegetationAttribute_h_
