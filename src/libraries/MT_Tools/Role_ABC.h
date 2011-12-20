// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Role_ABC.h $
// $Author: Jvt $
// $Modtime: 31/03/05 14:44 $
// $Revision: 3 $
// $Workfile: Role_ABC.h $
//
// *****************************************************************************

#ifndef __Role_ABC_h_
#define __Role_ABC_h_

#include <boost/serialization/export.hpp>
#include <boost/noncopyable.hpp>

namespace tools
{
// =============================================================================
// @class  Role_ABC
// Created: JVT/NLD 2004-08-03
// =============================================================================
class Role_ABC : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             Role_ABC() {}
    virtual ~Role_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

}

#endif // __Role_ABC_h_