#pragma warning ( disable: 4786 ) // identifier was truncated to '255' characters in the debug information

#ifndef __MIL_h_
#define __MIL_h_

#include "MT/MT_Tools/MT_Types.h"
#include "simulation_terrain/TER.h"

enum E_Tristate
{
    eTristate_True = 0,
    eTristate_False,
    eTristate_DontKnow,
};

/// $$$ NLD - Degueu à déplacer - ? Si pas possibilité d'encapsuler => mettre dans un 'Log.h'
class PHY_ComposanteTypePion;
typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;
typedef T_MaintenancePriorityVector::iterator        IT_MaintenancePriorityVector;
typedef T_MaintenancePriorityVector::const_iterator  CIT_MaintenancePriorityVector;

class MIL_Automate;
typedef std::vector< const MIL_Automate* > T_AutomateVector;
typedef T_AutomateVector::iterator         IT_AutomateVector;
typedef T_AutomateVector::const_iterator   CIT_AutomateVector;

class PHY_HumanWound;
typedef std::vector< const PHY_HumanWound* >     T_MedicalPriorityVector;
typedef T_MedicalPriorityVector::iterator        IT_MedicalPriorityVector;
typedef T_MedicalPriorityVector::const_iterator  CIT_MedicalPriorityVector;

class PHY_DotationCategory;
typedef std::map< const PHY_DotationCategory*, MT_Float /*rNbrToConvoy*/ > T_MerchandiseToConvoyMap;
typedef T_MerchandiseToConvoyMap::iterator                                 IT_MerchandiseToConvoyMap;
typedef T_MerchandiseToConvoyMap::const_iterator                           CIT_MerchandiseToConvoyMap;

// Useful types
class MIL_LimaOrder;

typedef std::vector< MIL_LimaOrder  > T_LimaVector;
typedef T_LimaVector::iterator        IT_LimaVector;
typedef T_LimaVector::const_iterator  CIT_LimaVector;

/// $$$ NLD - Degueu à déplacer

typedef uint32 MIL_MOSContextID;

namespace NEK
{
    class NEK_Input;
    class NEK_Output;
}

namespace boost
{
    namespace archive
    {
        class binary_oarchive;
        class binary_iarchive;
    };
};

typedef boost::archive::binary_iarchive MIL_CheckPointInArchive;
typedef boost::archive::binary_oarchive MIL_CheckPointOutArchive;

#include <boost/serialization/split_member.hpp>

std::ostream&    operator << ( std::ostream&, const MT_Vector3D& );
std::ostream&    operator << ( std::ostream&, const MT_Vector2D& );
NEK::NEK_Output& operator << ( NEK::NEK_Output& msg, const MT_Vector2D& vPos );
NEK::NEK_Input&  operator >> ( NEK::NEK_Input& msg, MT_Vector2D& vPos );

template<class T> std::ostream& operator << ( std::ostream& out, const std::vector< T >& vect )
{
    out << "{ ";
    if ( !vect.empty() )
    {
        std::copy( vect.begin(), vect.end() - 1, std::ostream_iterator<T>( out, ", " ) );
        out << vect.back() << ' ';
    }
    return out << '}';
}

template< class T, class U >
std::pair< T, U >& operator += ( std::pair< T, U >& lhs, const std::pair< T, U >& rhs )
{
    lhs.first  += rhs.first;
    lhs.second += rhs.second;

    return lhs;
}

template< class T, class U >
std::pair< T, U > operator + ( const std::pair< T, U >& lhs, const std::pair< T, U >& rhs )
{
    return std::make_pair( lhs.first + rhs.first, lhs.second + rhs.second );
}


#define MT_DELETERECYCLED(cont) std::for_each( WHOLE_CONT( cont ), DestroyRecycledObject() )
struct DestroyRecycledObject
{
    template< class T > 
    void operator()( T* ptr )
    {
        ptr->Destroy();
    }
};

// archive logger
#include "MT_Tools/MT_InputArchive_Logger.h"

// archive logger XML
class MT_XXmlInputArchive;
typedef MT_InputArchive_Logger< MT_XXmlInputArchive > MIL_InputArchive;

#pragma warning( disable: 4355 )

#endif // __MIL_h_
