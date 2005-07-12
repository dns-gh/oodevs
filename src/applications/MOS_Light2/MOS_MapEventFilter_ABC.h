// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapEventFilter_ABC.h $
// $Author: Ape $
// $Modtime: 5/10/04 18:33 $
// $Revision: 3 $
// $Workfile: MOS_MapEventFilter_ABC.h $
//
// *****************************************************************************

#ifndef __MOS_MapEventFilter_ABC_h_
#define __MOS_MapEventFilter_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif


class MOS_MapEventFilter_ABC;
typedef std::vector< MOS_MapEventFilter_ABC* >   T_MapEventFilterVector;
typedef T_MapEventFilterVector::reverse_iterator RIT_MapEventFilterVector;
typedef T_MapEventFilterVector::const_iterator   CIT_MapEventFilterVector;


// =============================================================================
/** @class  MOS_MapEventFilter_ABC
    @brief  MOS_MapEventFilter_ABC
*/
// Created: APE 2004-09-01
// =============================================================================
class MOS_MapEventFilter_ABC
{
    MT_COPYNOTALLOWED( MOS_MapEventFilter_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_MapEventFilter_ABC( bool bBlocking = true );
    virtual ~MOS_MapEventFilter_ABC();
    //@}

    //! @name Operations
    //@{
    void Push();
    void Pop();
    virtual void Draw() const;
    //@}

    //! @name Accessors
    //@{
    bool IsBlocking() const;
    virtual QObject* GetObject() = 0;
    //@}

private:
    //! @name Member data
    //@{
    bool bPushed_;
    bool bBlocking_;
    //@}
};

#include "MOS_MapEventFilter_ABC.inl"

#endif // __MOS_MapEventFilter_ABC_h_
