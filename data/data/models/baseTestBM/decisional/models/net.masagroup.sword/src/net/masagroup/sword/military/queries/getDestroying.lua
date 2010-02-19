queryImplementation "getDestroying" { ["execute"] = function ( params )
    local allRes
      allRes = {}
      -- Add position of myself, add position of all keypoints nearby
      for _, objective in pairs( params.entities ) do
        local res = {}
        if objective:isOfType( net.masagroup.military.generic.knowledge.Destroying ) then
          res[ #res + 1 ] = objective -- add objectif itself
        end
        allRes[ objective ] = res
      end
    return allRes
end}