// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamRadioBtnGroup.h $
// $Author: Ape $
// $Modtime: 7/06/04 17:05 $
// $Revision: 2 $
// $Workfile: ParamRadioBtnGroup.h $
//
// *****************************************************************************

#ifndef __ParamRadioBtnGroup_h_
#define __ParamRadioBtnGroup_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Param_ABC.h"


// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
template< class T >
class ParamRadioBtnGroup : public MT_ValuedRadioButtonGroup<T>, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamRadioBtnGroup );

public:
    //! @name Constructors/Destructor
    //@{
    ParamRadioBtnGroup( T& output, const std::string strLabel, int nStrips, Orientation orientation, QWidget* pParent, OptionalParamFunctor_ABC* pOptional = 0);
    ~ParamRadioBtnGroup();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    T& output_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< class T >
ParamRadioBtnGroup<T>::ParamRadioBtnGroup( T& output, const std::string strLabel, int nStrips, Orientation orientation, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : MT_ValuedRadioButtonGroup<T>( nStrips, orientation, strLabel.c_str(), pParent )
    , Param_ABC            ( pOptional )
    , output_                  ( output )
{
}


// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< class T >
ParamRadioBtnGroup<T>::~ParamRadioBtnGroup()
{
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup::WriteMsg
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< class T >
void ParamRadioBtnGroup<T>::WriteMsg( std::stringstream& strMsg )
{
    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    output_ = this->GetValue();
    strMsg << this->title().latin1() << ": " << this->GetValue();
}

#endif // __ParamRadioBtnGroup_h_
