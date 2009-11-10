

--SELECT sum(p.individuals) AS sum_indiv
--FROM   sword.population AS p, sword.relPopuPolygons AS r
--LEFT OUTER JOIN r.terrain_grid_id 
--ON     r.terrain_grid_id = p.cell_id

DROP TABLE IF EXISTS sword.sumIndividualByCell;

SELECT   p.cell_id, CAST(sum(p.individuals) AS INT) AS sum_indiv
INTO     sword.sumIndividualByCell
FROM     sword.population AS p, sword.terrain_grid AS t
WHERE    p.cell_id = t.id
GROUP BY p.cell_id
ORDER BY p.cell_id;
