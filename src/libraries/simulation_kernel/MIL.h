#pragma warning ( disable: 4786 ) // identifier was truncated to '255' characters in the debug information

#ifndef __MIL_h_
#define __MIL_h_

#include "simulation_terrain/TER.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"

enum E_Tristate
{
    eTristate_True = 0,
    eTristate_False,
    eTristate_DontKnow,
};

/// $$$ NLD - Degueu � d�placer - ? Si pas possibilit� d'encapsuler => mettre dans un 'Log.h'
class PHY_ComposanteTypePion;
typedef std::vector< const PHY_ComposanteTypePion* >   T_MaintenancePriorityVector;
typedef T_MaintenancePriorityVector::iterator         IT_MaintenancePriorityVector;
typedef T_MaintenancePriorityVector::const_iterator  CIT_MaintenancePriorityVector;

class PHY_HumanWound;
typedef std::vector< const PHY_HumanWound* >       T_MedicalPriorityVector;
typedef T_MedicalPriorityVector::iterator         IT_MedicalPriorityVector;
typedef T_MedicalPriorityVector::const_iterator  CIT_MedicalPriorityVector;

namespace boost
{
    namespace archive
    {
        class binary_oarchive;
        class binary_iarchive;
    };
};

#include <boost/serialization/split_member.hpp>

std::ostream& operator<<( std::ostream&, const MT_Vector3D& );
std::ostream& operator<<( std::ostream&, const MT_Vector2D& );

template< class T > std::ostream& operator << ( std::ostream& out, const std::vector< T >& vect )
{
    out << "{ ";
    if( !vect.empty() )
    {
        std::copy( vect.begin(), vect.end() - 1, std::ostream_iterator< T >( out, ", " ) );
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
std::pair< T, U > operator+( const std::pair< T, U >& lhs, const std::pair< T, U >& rhs )
{
    return std::make_pair( lhs.first + rhs.first, lhs.second + rhs.second );
}

#pragma warning( disable: 4355 )

#endif // __MIL_h_
