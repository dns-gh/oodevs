// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_VirtualObjectType.h $
// $Author: Nld $
// $Modtime: 27/10/04 17:09 $
// $Revision: 1 $
// $Workfile: MIL_VirtualObjectType.h $
//
// *****************************************************************************

#ifndef __MIL_VirtualObjectType_h_
#define __MIL_VirtualObjectType_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class MIL_VirtualObjectType_ControlZone;
class MIL_VirtualObject_ABC;

// =============================================================================
// @class  MIL_VirtualObjectType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_VirtualObjectType
{
    MT_COPYNOTALLOWED( MIL_VirtualObjectType )

public:
    //! @name 
    //@{
    typedef std::map< std::string, const MIL_VirtualObjectType*, sCaseInsensitiveLess > T_ObjectTypeMap;
    typedef T_ObjectTypeMap::const_iterator                                             CIT_ObjectTypeMap;

    static const MIL_VirtualObjectType_ControlZone& controlZone_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const MIL_VirtualObjectType* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName      () const;
          bool         IsInitialized() const;
    //@}

protected:
             MIL_VirtualObjectType( const std::string strName );
    virtual ~MIL_VirtualObjectType();

    //! @name Init tools
    //@{
    virtual void Read( xml::xistream& xis ) = 0;
    //@}
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadObject( xml::xistream& xis );
    //@}

protected:
    const std::string strName_;
          bool        bInitialized_;

private:
    static T_ObjectTypeMap objectTypes_;
};

#include "MIL_VirtualObjectType.inl"

#endif // __MIL_VirtualObjectType_h_
