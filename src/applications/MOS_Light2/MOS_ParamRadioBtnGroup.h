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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamRadioBtnGroup.h $
// $Author: Ape $
// $Modtime: 7/06/04 17:05 $
// $Revision: 2 $
// $Workfile: MOS_ParamRadioBtnGroup.h $
//
// *****************************************************************************

#ifndef __MOS_ParamRadioBtnGroup_h_
#define __MOS_ParamRadioBtnGroup_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Param_ABC.h"


// =============================================================================
/** @class  MOS_ParamRadioBtnGroup
    @brief  MOS_ParamRadioBtnGroup
    @par    Using example
    @code
    MOS_ParamRadioBtnGroup;
    @endcode
*/
// Created: APE 2004-04-20
// =============================================================================
template< class T >
class MOS_ParamRadioBtnGroup : public MT_ValuedRadioButtonGroup<T>, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamRadioBtnGroup );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamRadioBtnGroup( T& output, const std::string strLabel, int nStrips, Orientation orientation, QWidget* pParent );
    ~MOS_ParamRadioBtnGroup();
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
// Name: MOS_ParamRadioBtnGroup constructor
/** @param  output 
    @param  strLabel 
    @param  pParent 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< class T >
MOS_ParamRadioBtnGroup<T>::MOS_ParamRadioBtnGroup( T& output, const std::string strLabel, int nStrips, Orientation orientation, QWidget* pParent )
    : MT_ValuedRadioButtonGroup<T>( nStrips, orientation, strLabel.c_str(), pParent )
    , MOS_Param_ABC            ()
    , output_                  ( output )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamRadioBtnGroup destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< class T >
MOS_ParamRadioBtnGroup<T>::~MOS_ParamRadioBtnGroup()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_ParamRadioBtnGroup::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< class T >
void MOS_ParamRadioBtnGroup<T>::WriteMsg( std::stringstream& strMsg )
{
    output_ = this->GetValue();
    strMsg << this->title().latin1() << ": " << this->GetValue();
}


#ifdef MOS_USE_INLINE
#   include "MOS_ParamRadioBtnGroup.inl"
#endif

#endif // __MOS_ParamRadioBtnGroup_h_
