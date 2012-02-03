// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ObjectFilter.h $
// $Author: Nld $
// $Modtime: 26/10/04 16:00 $
// $Revision: 1 $
// $Workfile: MIL_ObjectFilter.h $
//
// *****************************************************************************

#ifndef __MIL_ObjectFilter_h_
#define __MIL_ObjectFilter_h_

#include <set>
#include <boost/functional/hash.hpp>

class DIA_Parameters;
class MIL_ObjectType_ABC;

// =============================================================================
// @class  MIL_ObjectFilter
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObjectFilter : private boost::noncopyable
{
public:
              MIL_ObjectFilter();
     explicit MIL_ObjectFilter( const std::vector< std::string >& params );
     virtual ~MIL_ObjectFilter();

    //! @name
    //@{
            void Set  ( const std::string& type );
            void Reset( const std::string& type );
    virtual bool Test ( const MIL_ObjectType_ABC& type ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set<std::string> T_ObjectStateMap;
    //@}

private:
    T_ObjectStateMap objects_;
};

class MIL_DangerousObjectFilter : public MIL_ObjectFilter
{
    virtual bool Test( const MIL_ObjectType_ABC& type ) const;
};

#endif // __MIL_ObjectFilter_h_
