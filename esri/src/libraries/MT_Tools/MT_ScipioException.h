//*****************************************************************************
//
// $Created: JVT 03-02-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_ScipioException.h $
// $Author: Age $
// $Modtime: 14/04/05 10:33 $
// $Revision: 4 $
// $Workfile: MT_ScipioException.h $
//
//*****************************************************************************

#ifndef __MT_ScipioException_h_
#define __MT_ScipioException_h_

#include "MT_Tools_Types.h"

#include "MT/MT_Tools/MT_Exception.h"

//*****************************************************************************
// Created: JVT 03-02-04
//*****************************************************************************
class MT_ScipioException : public MT_Exception
{
public:
     MT_ScipioException( const std::string& strContext, 
                         const std::string& strFile, 
                         uint nLine, 
                         const std::string& strMessage, 
                         const std::string& strDescription );

     MT_ScipioException( const std::string& strContext, 
                         const std::string& strFile, 
                         uint nLine, 
                         const std::string& strMessage );

    MT_ScipioException( const MT_ScipioException& );
    MT_ScipioException& operator = ( const MT_ScipioException& );

    ~MT_ScipioException();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const std::string& GetContext    () const;
    const std::string& GetFile       () const;
    const std::string& GetMsg        () const; 
    const std::string& GetDescription() const; 
    uint               GetLine       () const;

    void               SendToLogger() const;
    //@}

private:
    std::string strContext_; 
    std::string strFile_;
    uint        nLine_;
    std::string strMessage_; 
    std::string strDescription_;
};

#include "MT_ScipioException.inl"

#endif // __MT_ScipioException_h_