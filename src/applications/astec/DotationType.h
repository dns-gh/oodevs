//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __DotationType_h_
#define __DotationType_h_

#include "Types.h"
#include "RawVisionData.h"

class Agent;
enum E_PerceptionResult;

//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class DotationType
{
    MT_COPYNOTALLOWED( DotationType )

public:
    DotationType( const std::string& strCategory, const std::string& strName, InputArchive& archive );
    ~DotationType();

    //! @name Accessors
    //@{
    const std::string& GetCategory             () const;
    const std::string& GetName                 () const;
          uint         GetID                   () const;
          bool         CanBeUsedForIndirectFire() const;

    //@}


private:
    const std::string strCategory_;
    const std::string strName_;
          uint        nID_;
          bool        bCanBeUsedForIndirectFire_;
};

#include "DotationType.inl"

#endif // __DotationType_h_