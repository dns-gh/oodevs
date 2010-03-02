queryImplementation "getDestroying" { ["execute"] = function ( params )
    local allRes
      allRes = {}
      -- Add position of myself, add position of all keypoints nearby
      for _, objective in pairs( params.entities ) do
        if objective:isOfType( net.masagroup.military.generic.knowledge.Destroying ) then
          allRes[ #allRes + 1 ] = objective -- add objectif itself
        end
      end
    return allRes
end}