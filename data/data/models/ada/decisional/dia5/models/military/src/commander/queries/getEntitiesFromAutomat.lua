queryImplementation "getEntitiesFromAutomat"
{ 
    [ "execute" ] = function ( params )
        return integration.getEntitiesFromAutomat( params.automat, params.role, params.withPC)
    end
}