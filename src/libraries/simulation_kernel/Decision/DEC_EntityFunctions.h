#ifndef __DEC_EntityFunctions_h_
#define __DEC_EntityFunctions_h_

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;

// =============================================================================
/** @class  DEC_EntityFunctions
    @brief  DEC functions for a MIL_Entity_ABC
*/
// =============================================================================
class DEC_EntityFunctions
{
public:
    static void Register( sword::Brain& brain );

    static bool IsAgent( const DEC_Decision_ABC* actor );
    static bool IsAutomaton( const DEC_Decision_ABC* actor );
    static bool IsCrowd( const DEC_Decision_ABC* actor );

    static void DecisionalState( const DEC_Decision_ABC* actor, const std::string& key, const std::string& value );
};

#endif // __DEC_EntityFunctions_h_
