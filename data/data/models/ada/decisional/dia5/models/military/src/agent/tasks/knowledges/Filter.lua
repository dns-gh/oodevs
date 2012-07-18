local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local bodySearchStrength = companyTask:getBodySearchStrength( params )
        local blockingStrength = companyTask:getBlockingStrength( params )
        if bodySearchStrength ~= NIL and bodySearchStrength ~= nil then
            -- for filtering
            return { position = companyTask:getPosition( params ),
                     bodySearchStrength = companyTask:getBodySearchStrength( params ),
                     blockingStrength = companyTask:getBlockingStrength( params )}
         else
            -- for denying
            return { position = companyTask:getPosition( params ) }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Filter"] = t

return result
