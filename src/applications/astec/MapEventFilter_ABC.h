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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapEventFilter_ABC.h $
// $Author: Ape $
// $Modtime: 5/10/04 18:33 $
// $Revision: 3 $
// $Workfile: MapEventFilter_ABC.h $
//
// *****************************************************************************

#ifndef __MapEventFilter_ABC_h_
#define __MapEventFilter_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif


class MapEventFilter_ABC;
typedef std::vector< MapEventFilter_ABC* >   T_MapEventFilterVector;
typedef T_MapEventFilterVector::reverse_iterator RIT_MapEventFilterVector;
typedef T_MapEventFilterVector::const_iterator   CIT_MapEventFilterVector;


// =============================================================================
/** @class  MapEventFilter_ABC
    @brief  MapEventFilter_ABC
*/
// Created: APE 2004-09-01
// =============================================================================
class MapEventFilter_ABC
{
    MT_COPYNOTALLOWED( MapEventFilter_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             MapEventFilter_ABC( bool bBlocking = true );
    virtual ~MapEventFilter_ABC();
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

#include "MapEventFilter_ABC.inl"

#endif // __MapEventFilter_ABC_h_
