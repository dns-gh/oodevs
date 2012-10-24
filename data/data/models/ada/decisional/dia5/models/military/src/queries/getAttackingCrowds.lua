queryImplementation "getAttackingCrowds" 
{ 
    [ "execute" ] = function( params )
        local atackingCrowds = {}
        local crowds = integration.getKnowledgesCrowdsEngaging()
        for _,crowd in pairs( crowds ) do
          atackingCrowds[ #atackingCrowds + 1 ] = CreateKnowledge( world.Crowd, crowd )
        end
        return atackingCrowds
    end
}
