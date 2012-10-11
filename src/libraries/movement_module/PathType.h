//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/PathType.h $
// $Author: Nld $
// $Modtime: 21/07/05 16:33 $
// $Revision: 5 $
// $Workfile: PathType.h $
//
//*****************************************************************************

#ifndef movement_module_PathType_h_
#define movement_module_PathType_h_

#include <boost/noncopyable.hpp>
#include <cassert>
#include <string>
#include <vector>
#include <map>

namespace sword
{
namespace movement
{
//*****************************************************************************
// Created: JDY 03-04-10
//*****************************************************************************
class PathType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    static const PathType movement_;
    static const PathType recon_;
    static const PathType infiltration_;
    static const PathType assault_;
    static const PathType retreat_;
    static const PathType backup_;
    static const PathType mineClearance_;
    static const PathType logistic_;
    static const PathType criminal_;
    static const PathType nbc_;

    typedef std::map< std::string, const PathType* > T_PathTypeMap;
    typedef T_PathTypeMap::const_iterator              CIT_PathTypeMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();

    static       T_PathTypeMap& GetPathTypes();
    static const PathType*  Find        ( const std::string& strName );
    static const PathType*  Find        ( unsigned int nID );
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
        eCriminal,
        eNBC
    };

    typedef std::vector< const PathType* >  T_PathTypeFromIDVector;
    //@}

private:
             PathType( E_PathType nType, const std::string& strName );
    virtual ~PathType();

private:
    const E_PathType  nPathType_;
    const std::string strName_;

private:
    static T_PathTypeFromIDVector pathTypesFromID_;
    static T_PathTypeMap          pathTypes_;
};

}
}

#endif // movement_module_PathType_h_
