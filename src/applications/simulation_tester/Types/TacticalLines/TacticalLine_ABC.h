// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "Types.h"
#include "Tools/Position.h"

namespace TEST {

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  TacticalLine_ABC
*/
// Created: SBO 2005-08-09
// =============================================================================
class TacticalLine_ABC
{

public:
    //! @name Enumerations
    //@{
    enum E_LineType
    {
        eLima,
        eLimit
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC();
             TacticalLine_ABC( const T_PositionVector& points );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Accessors
    //@{
            T_EntityId GetId      () const;
    virtual E_LineType GetLineType() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateToSim() = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );            //!< Copy constructor
    TacticalLine_ABC& operator=( const TacticalLine_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    void ReadPoints( XmlInputArchive& archive );
    //@}

protected:
    //! @name Member data
    //@{
    T_EntityId       nId_;
    T_PositionVector points_;
    bool             bIsSyncWithSim_;
    //@}
};

} // end namespace TEST

#include "TacticalLine_ABC.inl"

#endif // __TacticalLine_ABC_h_
