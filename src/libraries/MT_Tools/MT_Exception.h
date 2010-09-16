// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_Exception_h_
#define __MT_Exception_h_

#include <exception>
#include <string>

//=============================================================================
/** @class  MT_Exception
    @brief  Exception
*/
// Created:  NLD 00-02-22
//=============================================================================
class MT_Exception : public std::exception
{
public:
    //-------------------------------------------------------------------------
    /** @name Constructors / Destructor */
    //-------------------------------------------------------------------------
    //@{
             MT_Exception( const std::string& strContext, int nCode, const std::string& strInfo );
    virtual ~MT_Exception() throw();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    int                 GetCode   () const;
    const std::string&  GetContext() const;
    const std::string&  GetInfo   () const;

    virtual const char* what() const throw();
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name Member data */
    //-------------------------------------------------------------------------
    //@{
    std::string strContext_;
    int         nCode_;
    std::string strInfo_;
    //@}
};

#endif // __MT_Exception_h_
