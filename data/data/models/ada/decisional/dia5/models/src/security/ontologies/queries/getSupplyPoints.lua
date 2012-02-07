queryImplementation "getSupplyPoints" 
{ 
    [ "execute" ] = function ( params )
        local result = {}
        local knowledges = DEC_ObjectKnowledgesInCircle( 1000000, { "supply point" } )
        for _, knowledge in pairs( knowledges ) do
            result[ #result + 1 ] = CreateKnowledge( security.ontologies.classes.Object, knowledge )              
        end
        return result
    end
}
