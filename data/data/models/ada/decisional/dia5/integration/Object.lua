integration.createResourceLinkWithObject = function( object, resourceNode )
    DEC_Trace( "object = "..tostring(  object ) )
    DEC_Trace( "resourceNode = "..tostring(  resourceNode ) )
    return DEC_ResourceNetwork_CreateLinkFromObject( object.source, resourceNode.source )
end