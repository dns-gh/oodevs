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

#include <hash_set>
#include <boost/functional/hash.hpp>

class DIA_Parameters;
class MIL_ObjectType_ABC;

// =============================================================================
// @class  MIL_ObjectFilter
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObjectFilter
{
    MT_COPYNOTALLOWED( MIL_ObjectFilter )

public:
     MIL_ObjectFilter();
     MIL_ObjectFilter( DIA_Parameters& diaParameters, uint nFirstDiaParam );     
    ~MIL_ObjectFilter();

    //! @name 
    //@{
            void Set  ( const std::string& type );
            void Reset( const std::string& type );
    virtual bool Test ( const MIL_ObjectType_ABC& type ) const;
    //@}

private:
    //! @name Types
    //@{
    template<class Key>
    class hash_compare
    {
    public:
         static const std::size_t bucket_size = 4;
         static const std::size_t min_buckets = 8;

         hash_compare() : hasher_ () 
         {
         }
         
         std::size_t operator()(const Key& k) const 
         {
             return hasher_( k );
         }

         bool operator()( const Key& k1, const Key& k2 ) const 
         {
             return k1 < k2;
         }
    private:
        boost::hash< Key >  hasher_;
    };

    typedef stdext::hash_set<std::string, hash_compare<std::string> > T_ObjectStateMap; 
    //@}

private:
    T_ObjectStateMap objects_;
};

#endif // __MIL_ObjectFilter_h_
