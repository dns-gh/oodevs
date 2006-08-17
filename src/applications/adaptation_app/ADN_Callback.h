// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Callback.h $
// $Author: Ape $
// $Modtime: 4/04/05 11:19 $
// $Revision: 1 $
// $Workfile: ADN_Callback.h $
//
// *****************************************************************************

#ifndef __ADN_Callback_h_
#define __ADN_Callback_h_


// =============================================================================
/** @class  ADN_Callback_ABC
    @brief  ADN_Callback_ABC
*/
// Created: APE 2005-04-04
// =============================================================================
template< typename Result >
class ADN_Callback_ABC
{
public:
    ADN_Callback_ABC() {}
    virtual Result operator()() = 0;
};


// =============================================================================
/** @class  ADN_Callback
    @brief  ADN_Callback
*/
// Created: APE 2005-04-04
// =============================================================================
template< typename Result, typename Class >
class ADN_Callback 
    : public ADN_Callback_ABC< Result >
{
public:
    ADN_Callback( Class* pCaller, Result( Class::* f )( ) ) : f_( f ), pCaller_( pCaller ) {}

    Result operator()()
    {
        return f_( pCaller_ );
    }

private:
    std::mem_fun_t<Result, Class> f_;
    Class* pCaller_;
};


#endif // __ADN_Callback_h_
