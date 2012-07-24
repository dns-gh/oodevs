local result = 
{
    fillParameters = function( self, companyTask, params )        
         local ficelleDeVol = companyTask:getFicelleDeVol( params )
         return { ficelleDeVol = ficelleDeVol }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.rens.tasks.knowledges.ROIM_RenseignerSur"] = t

return result
