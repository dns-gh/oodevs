// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObjectAttribute_ABC_h_
#define __UrbanObjectAttribute_ABC_h_

namespace MsgsSimToClient
{
    class MsgUrbanAttributes;
}

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  UrbanObjectAttribute_ABC
@brief  UrbanObjectAttribute_ABC
*/
// Created: SLG 2009-12-04
// =============================================================================
class UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    UrbanObjectAttribute_ABC( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual ~UrbanObjectAttribute_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const MsgsSimToClient::MsgUrbanAttributes& message ) = 0;
    virtual void Send     (       MsgsSimToClient::MsgUrbanAttributes& message ) const = 0;
    virtual void AsnDelete(       MsgsSimToClient::MsgUrbanAttributes& message ) const = 0;
    //@}

protected:
    unsigned int nType_;
};

}

#endif // __UrbanObjectAttribute_ABC_h_
