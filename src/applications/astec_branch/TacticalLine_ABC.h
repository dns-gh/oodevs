// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TacticalLine_ABC.h $
// $Author: Ape $
// $Modtime: 28/07/04 14:48 $
// $Revision: 5 $
// $Workfile: TacticalLine_ABC.h $
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "Order_Def.h"
#include "Types.h"
#include "ASN_Types.h"

class CoordinateConverter_ABC;
class GlTools_ABC;
class ActionController;

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
             TacticalLine_ABC( const std::string& baseName, unsigned long id, const CoordinateConverter_ABC& converter );
             TacticalLine_ABC( const std::string& baseName, unsigned long id, const T_PointVector& points, const CoordinateConverter_ABC& converter );
             TacticalLine_ABC( const std::string& baseName, unsigned long id, const ASN1T_Line& line, const CoordinateConverter_ABC& converter );
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
        message.Send( (MIL_MOSContextID)this );
        nNetworkState_ = eNetworkStateRegistering;
    }
    void DrawName( const GlTools_ABC& tools ) const;
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

    unsigned long  id_;
    T_PointVector  pointList_;
    std::string    strName_;
    E_State        nState_;
    E_NetworkState nNetworkState_;
    bool           bCreatedBy; 
    //@}
};

#endif // __TacticalLine_ABC_h_
