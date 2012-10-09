queryImplementation "getDefaultEntitiesToLead"
{ 
    ["execute"] = function ( params )
        return integration.defaultPlatoonInCompany()
    end
}