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

#include "astec_kernel/Types.h"
#include "ASN_Types.h"

class CoordinateConverter_ABC;
class GlTools_ABC;
class ActionController;
class Publisher_ABC;

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( const std::string& baseName, unsigned long id, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher );
             TacticalLine_ABC( const std::string& baseName, unsigned long id, const T_PointVector& points, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher );
             TacticalLine_ABC( const std::string& baseName, unsigned long id, const ASN1T_Line& line, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    void Delete();

    void Update( const ASN1T_MsgLimitCreationAck& asnMsg );
    void Update( const ASN1T_MsgLimitUpdateAck& asnMsg );
    void Update( const ASN1T_MsgLimaCreationAck& asnMsg );
    void Update( const ASN1T_MsgLimaUpdateAck& asnMsg);

    virtual void Draw( const GlTools_ABC& tools ) const;

    void UpdateToSim();

    bool IsAt( const geometry::Point2f& point, float precision = 100.f ) const;

    virtual void Select( ActionController& actions ) const = 0;
    virtual void ContextMenu( ActionController& actions, const QPoint& point ) const = 0;
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    std::string   GetName() const;

    bool IsUpdatingToSim() const;
    bool IsCreatedByMos() const;
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
    void DrawName( const GlTools_ABC& tools ) const;
    template< typename Ack >
    void ValidateAcknowledge( const Ack& ack );
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
    const CoordinateConverter_ABC& converter_;
    Publisher_ABC& publisher_;

    unsigned long  id_;
    T_PointVector  pointList_;
    std::string    strName_;
    E_State        nState_;
    E_NetworkState nNetworkState_;
    bool           bCreatedBy; 
    //@}
};

#endif // __TacticalLine_ABC_h_
