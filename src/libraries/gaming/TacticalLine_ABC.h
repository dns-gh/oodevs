// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "clients_kernel/Types.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "ASN_Types.h"

namespace kernel
{
    class GlTools_ABC;
    class ActionController;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Publisher_ABC;

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC : public kernel::TacticalLine_ABC
                       , public kernel::Extension_ABC
                       , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( const QString& baseName, unsigned long id, Publisher_ABC& publisher );
             TacticalLine_ABC( xml::xistream& xis, Publisher_ABC& publisher );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    void Delete();
    void Polish(); // $$$$ SBO 2006-11-06: EntityImplementation?

    void Update( const ASN1T_MsgLimitCreationAck& asnMsg );
    void Update( const ASN1T_MsgLimitUpdateAck& asnMsg );
    void Update( const ASN1T_MsgLimaCreationAck& asnMsg );
    void Update( const ASN1T_MsgLimaUpdateAck& asnMsg);

    void UpdateToSim();
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual QString       GetName() const;

    bool IsUpdatingToSim() const;
    bool IsCreatedByMos() const;
    virtual bool IsLimit() const = 0;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {   
        eStateOk            = 0x00,
        eStateCreated       = 0x01,
        eStateModified      = 0x02,
        eStateDeleted       = 0x04
    };

    enum E_NetworkState
    {   
        eNetworkStateNotRegistered     = 0,
        eNetworkStateRegistering       = 1,
        eNetworkStateRegistered        = 2
    };
    //@}

    //! @name Helpers
    //@{
    void WriteGeometry( ASN1T_Line& line );
    virtual void UpdateToSim( E_State state ) = 0;
    template< typename Message >
    void Send( Message& message )
    {
        message.Send( publisher_, (unsigned long)this );
        nNetworkState_ = eNetworkStateRegistering;
    }

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    template< typename Ack >
    void ValidateAcknowledge( const Ack& ack );
    void ReadPoint( xml::xistream& xis );
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
    Publisher_ABC& publisher_;

    unsigned long  id_;
    QString        strName_;
    E_State        nState_;
    E_NetworkState nNetworkState_;
    bool           bCreatedBy; 
    //@}
};

#endif // __TacticalLine_ABC_h_
