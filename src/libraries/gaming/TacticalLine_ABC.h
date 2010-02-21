// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

class Publisher_ABC;

// $$$$ _RC_ FDS 2010-01-27: n doit pouvoir supprimer
//struct ASN1T_MsgLimaUpdate;
//struct ASN1T_MsgLimitUpdate;


// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC : public kernel::TacticalLine_ABC
                       , public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgLimaUpdate >
                       , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgLimitUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( const QString& baseName, unsigned long id, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    void Delete();
    void Create();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId  () const;
    virtual QString       GetName() const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {   
        eStateOk       = 0x00,
        eStateCreated  = 0x01,
        eStateModified = 0x02,
        eStateDeleted  = 0x04
    };
    //@}

    //! @name Helpers
    //@{
    void WriteGeometry( Common::MsgLocation& loc ) const;
    void WriteDiffusion( Common::MsgTacticalLine::Diffusion& diffusion ) const;
    virtual void UpdateToSim( E_State state ) = 0;
    template< typename Message >
    void Send( Message& message )
    {
        message.Send( publisher_);
    }

    virtual void DoUpdate( const MsgsMessengerToClient::MsgLimaUpdate& message );
    virtual void DoUpdate( const MsgsMessengerToClient::MsgLimitUpdate&  message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );
    TacticalLine_ABC& operator=( const TacticalLine_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    Publisher_ABC& publisher_;
    unsigned long  id_;
    QString        name_;
    //@}
};

#endif // __TacticalLine_ABC_h_
