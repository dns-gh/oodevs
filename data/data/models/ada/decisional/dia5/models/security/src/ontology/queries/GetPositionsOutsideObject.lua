-- ****************************************************************************
-- GetPositionsOutsideObject
-- Get a position outside the given object
-- ****************************************************************************
local CheckEqualObjects = function( mlObject1, mlObject2 )
    if not DEC_IsValidKnowledgeObject( mlObject1.source )or not DEC_IsValidKnowledgeObject( mlObject2.source ) then
        return false
    end
    if DEC_Geometrie_PositionsEgales( 
       DEC_Geometrie_CalculerBarycentreLocalisation( DEC_ConnaissanceObjet_Localisation( mlObject1.source ) ),
       DEC_Geometrie_CalculerBarycentreLocalisation( DEC_ConnaissanceObjet_Localisation( mlObject2.source ) ) ) then
        return true
    end
    return false
end

queryImplementation "GetPositionsOutsideObject" 
{ 
    [ "execute" ] = function ( params )
        local result = knowledgeManager.getQueryResult( "GetPositionsOutsideObject" )
        if next( result ) then
            if not DEC_IsValidKnowledgeObject( params.object.source ) 
               or CheckEqualObjects( params.object, result[ 2 ] )  then
                return { result[ 1 ] }
            end
        end
        result = {}
        local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( params.object.source ) , 10 )
        local positionOnLocalisation = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
        local positionOutsideObject = CreateKnowledge( ontology.classes.Position, positionOnLocalisation ) 

        knowledgeManager.setQueryResult( "GetPositionsOutsideObject", { positionOutsideObject, params.object } )
        return { positionOutsideObject }
    end
}