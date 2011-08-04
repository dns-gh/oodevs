// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-08-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_ValueConvertor.h $
// $Author: Ape $
// $Modtime: 17/12/04 11:49 $
// $Revision: 1 $
// $Workfile: GQ_ValueConvertor.h $
//
// *****************************************************************************

#ifndef __GQ_ValueConvertor_h_
#define __GQ_ValueConvertor_h_

// =============================================================================
/** @class  GQ_ValueConvertor
    @brief  GQ_ValueConvertor
    @par    Using example
    @code
    GQ_ValueConvertor;
    @endcode
*/
// Created: CBX 2003-08-19
// =============================================================================
template< class T_Input, class T_Output >
class GQ_ValueConvertor
{
public:
    //! @name Constructors/Destructor
    //@{
    GQ_ValueConvertor();
    virtual ~GQ_ValueConvertor();
    //@}

    //! @name Accessors
    //@{
    T_Output GetConversion( T_Input ) const;

    virtual void Convert( T_Input, T_Output& ) const = 0;
    //@}
};

// =============================================================================
// Template methods implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: GQ_ValueConvertor constructor
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
template< class T_Input, class T_Output >
GQ_ValueConvertor< T_Input, T_Output >::GQ_ValueConvertor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GQ_ValueConvertor destructor
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
template< class T_Input, class T_Output >
GQ_ValueConvertor< T_Input, T_Output >::~GQ_ValueConvertor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GQ_ValueConvertor< T_Input, T_Output >::GetConversion
/** @param
    @return
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
template< class T_Input, class T_Output >
T_Output GQ_ValueConvertor< T_Input, T_Output >::GetConversion( T_Input inputValue ) const
{
    T_Output outputValue;
    Convert( inputValue, outputValue );

    return outputValue;
}

#endif // __GQ_ValueConvertor_h_
