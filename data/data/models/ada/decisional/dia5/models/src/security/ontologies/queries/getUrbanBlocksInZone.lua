queryImplementation "getUrbanBlocksInZone" 
{ 
    [ "execute" ] = function ( params )
        local result = {}
        local urbanKnowledges = DEC_Connaissances_BlocUrbainDansZone( params.zone.source )
        for _, urbanKnowledge in pairs( urbanKnowledges ) do
            result[ #result + 1 ] = CreateKnowledge( security.ontologies.classes.UrbanBlock, urbanKnowledge )              
        end
        return result
    end
}

