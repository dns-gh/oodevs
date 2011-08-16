//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.h $
// $Author: Nld $
// $Modtime: 21/07/05 16:33 $
// $Revision: 5 $
// $Workfile: DEC_PathType.h $
//
//*****************************************************************************

#ifndef __DEC_PathType_h_
#define __DEC_PathType_h_

#include "MIL.h"
#include "MT_Tools/MT_String.h"

//*****************************************************************************
// Created: JDY 03-04-10
//*****************************************************************************
class DEC_PathType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    static const DEC_PathType movement_;
    static const DEC_PathType recon_;
    static const DEC_PathType infiltration_;
    static const DEC_PathType assault_;
    static const DEC_PathType retreat_;
    static const DEC_PathType backup_;
    static const DEC_PathType mineClearance_;
    static const DEC_PathType logistic_;
    static const DEC_PathType criminal_;

    typedef std::map< std::string, const DEC_PathType* > T_PathTypeMap;
    typedef T_PathTypeMap::const_iterator              CIT_PathTypeMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static       T_PathTypeMap& GetPathTypes();
    static const DEC_PathType*  Find        ( const std::string& strName );
    static const DEC_PathType*  Find        ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
          unsigned int         GetID  () const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_PathType
    {
        eMovement = 0,
        eRecon,
        eInfiltration,
        eAssault,
        eRetreat,
        eBackup,
        eMineClearance,
        eLogistic,
        eCriminal
    };

    typedef std::vector< const DEC_PathType* >  T_PathTypeFromIDVector;
    //@}

private:
     DEC_PathType( E_PathType nType, const std::string& strName );
    virtual ~DEC_PathType();

private:
    const E_PathType  nPathType_;
    const std::string strName_;

private:
    static T_PathTypeFromIDVector pathTypesFromID_;
    static T_PathTypeMap          pathTypes_;
};

#include "DEC_PathType.inl"

#endif // __DEC_PathType_h_
