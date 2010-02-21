// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SoilAttribute_h_
#define __SoilAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace MsgsSimToClient
{
    class MsgUrbanAttributes;
}

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  SoilAttribute
@brief  SoilAttribute
*/
// Created: SLG 2009-12-04
// =============================================================================
class SoilAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SoilAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual ~SoilAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual void Send     (       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    virtual void AsnDelete(       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    //@}

private:
    //! @name Data members
    //@{
    float       occupation_;
    float       trafficability_;
    bool        isMultiple_;
    std::string compoundClearing_;
    //@}
};

}

#endif // __SoilAttribute_h_
