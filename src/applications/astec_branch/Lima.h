//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Lima.h $
// $Author: Ape $
// $Modtime: 26/01/05 10:22 $
// $Revision: 6 $
// $Workfile: Lima.h $
//
//*****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"
#include "IDManager.h"
#include "ASN_Types.h"

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
             Lima( Controller& controller, const CoordinateConverter& converter );
             Lima( Controller& controller, const T_PointVector& pointList, E_FuncLimaType nFuncType, const CoordinateConverter& converter );
             Lima( Controller& controller, const ASN1T_MsgLimaCreation& asnMsg, const CoordinateConverter& converter );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    virtual void Select( ActionController& actions ) const;
    virtual void ContextMenu( ActionController& actions, const QPoint& point ) const;
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
    Controller& controller_;
    E_FuncLimaType   nFuncType_;
    //@}
};

#endif // __Lima_h_
