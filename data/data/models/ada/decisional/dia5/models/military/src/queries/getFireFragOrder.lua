queryImplementation "queries.getFireFragOrder" 
{
    [ "execute" ] = function ( params )
        return integration.getFireFragOrder() -- /!\ can return nil
    end
}