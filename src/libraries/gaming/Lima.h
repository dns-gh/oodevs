// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"
#include "clients_kernel/IDManager.h"
#include "Types.h"

class Controller;

// =============================================================================
/** @class  Lima
    @brief  Represents a lima.
*/
// Created: APE 2004-03-12
// =============================================================================
class Lima : public TacticalLine_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
             Lima( Controller& controller, Publisher_ABC& publisher, const CoordinateConverter_ABC& converter );
             Lima( Controller& controller, Publisher_ABC& publisher, const T_PointVector& pointList, E_FuncLimaType nFuncType, const CoordinateConverter_ABC& converter );
             Lima( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimaCreation& asnMsg, const CoordinateConverter_ABC& converter );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const GlTools_ABC& tools ) const;
    virtual void Select( ActionController& actions ) const;
    virtual void ContextMenu( ActionController& actions, const QPoint& point ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lima( const Lima& );
    Lima& operator=( const Lima& );
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void FillAndSend();
    virtual void UpdateToSim( E_State state );
    //@}

public:
    //! @name Member data
    //@{
    static IDManager idManager_;

private:
    Controller& controller_;
    E_FuncLimaType   nFuncType_;
    //@}
};

#endif // __Lima_h_
