// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Param_ABC.h $
// $Author: Ape $
// $Modtime: 3/08/04 17:31 $
// $Revision: 5 $
// $Workfile: Param_ABC.h $
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

class ActionContext;
class QPopupMenu;

class Param_ABC;
typedef std::vector< Param_ABC* >   T_ParamVector;
typedef T_ParamVector::iterator         IT_ParamVector;
typedef T_ParamVector::const_iterator   CIT_ParamVector;

// =============================================================================
// Created: APE 2004-03-18
// =============================================================================
class Param_ABC
{
    MT_COPYNOTALLOWED( Param_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             Param_ABC( bool bOptional );
    virtual ~Param_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Draw();
    virtual void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    virtual bool CheckValidity();
    virtual void WriteMsg ( std::stringstream& strMsg ) = 0;
    //@}

    //! @name Accessors
    //@{
    bool IsOptional() const;
    //@}

private:
    bool bIsOptional_;
};

#   include "Param_ABC.inl"

#endif // __Param_ABC_h_
