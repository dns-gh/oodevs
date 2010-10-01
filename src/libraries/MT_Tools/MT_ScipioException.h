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

#include <string>

//*****************************************************************************
// Created: JVT 03-02-04
//*****************************************************************************
class MT_ScipioException
{
public:
     MT_ScipioException( const std::string& strContext,
                         const std::string& strFile,
                         unsigned int nLine,
                         const std::string& strMessage,
                         const std::string& strDescription );

     MT_ScipioException( const std::string& strContext,
                         const std::string& strFile,
                         unsigned int nLine,
                         const std::string& strMessage );

    virtual ~MT_ScipioException();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const std::string& GetContext    () const;
    const std::string& GetFile       () const;
    const std::string& GetMsg        () const;
    const std::string& GetDescription() const;
    unsigned int       GetLine       () const;

    void               SendToLogger() const;
    //@}

private:
    std::string strContext_;
    std::string strFile_;
    unsigned int nLine_;
    std::string strMessage_;
    std::string strDescription_;
};

#include "MT_ScipioException.inl"

#endif // __MT_ScipioException_h_
