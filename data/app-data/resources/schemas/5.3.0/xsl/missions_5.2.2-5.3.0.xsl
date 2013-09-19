<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <!-- matches descendant-or-self:node() -->
  <xsl:template match="parameter">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <xsl:if test="./@type = 'bouchon mines'">
              <xsl:attribute name="type">mines</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone minee lineaire'">
              <xsl:attribute name="type">mined area (linear)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone minee par dispersion'">
              <xsl:attribute name="type">mined area (scattered)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'fosse anti char'">
              <xsl:attribute name="type">anti tank obstacle</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'abattis'">
              <xsl:attribute name="type">abattis</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'barricade'">
              <xsl:attribute name="type">barricade</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'eboulement'">
              <xsl:attribute name="type">landslide</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'destruction route'">
              <xsl:attribute name="type">road destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'destruction pont'">
              <xsl:attribute name="type">bridge destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'pont flottant continu'">
              <xsl:attribute name="type">continuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'poste tir'">
              <xsl:attribute name="type">firing post</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone protegee'">
              <xsl:attribute name="type">safe zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone implantation canon'">
              <xsl:attribute name="type">gun artillery deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone implantation cobra'">
              <xsl:attribute name="type">cobra deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone implantation lrm'">
              <xsl:attribute name="type">mrls deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'site franchissement'">
              <xsl:attribute name="type">crossing site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'nuage nbc'">
              <xsl:attribute name="type">nbc cloud</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'plot ravitaillement'">
              <xsl:attribute name="type">logistic support area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'site decontamination'">
              <xsl:attribute name="type">decontamination site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'rota'">
              <xsl:attribute name="type">rota area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone nbc'">
              <xsl:attribute name="type">nbc zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone brouillage'">
              <xsl:attribute name="type">jamming area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'aire poser'">
              <xsl:attribute name="type">landing area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'piste'">
              <xsl:attribute name="type">trail</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'plate-forme'">
              <xsl:attribute name="type">platform</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone mobilite amelioree'">
              <xsl:attribute name="type">mobility enhanced area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone poser helicoptere'">
              <xsl:attribute name="type">landing area (helicopter)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'aire logistique'">
              <xsl:attribute name="type">logistic area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'camp prisonniers'">
              <xsl:attribute name="type">prisoners camp</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'camp refugies'">
              <xsl:attribute name="type">refugees camp</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'poste controle'">
              <xsl:attribute name="type">checkpoint</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'terrain largage'">
              <xsl:attribute name="type">dropping area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone interdite mouvement'">
              <xsl:attribute name="type">movement forbidden area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone interdite tir'">
              <xsl:attribute name="type">firing forbidden area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone implantation mortier'">
              <xsl:attribute name="type">mortar deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'pont flottant discontinu'">
              <xsl:attribute name="type">discontinuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'installation'">
              <xsl:attribute name="type">installation</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'bloc urbain'">
              <xsl:attribute name="type">urban block</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'zone bouclee'">
              <xsl:attribute name="type">seal off area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'incendie'">
              <xsl:attribute name="type">fire</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'mouvement population'">
              <xsl:attribute name="type">population moving</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'station de metro'">
              <xsl:attribute name="type">metro station</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'noeud de reseau de ressource'">
              <xsl:attribute name="type">ressource network element</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'souterrain egout'">
              <xsl:attribute name="type">sewer entry</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'autoroute'">
              <xsl:attribute name="type">motorway</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'chemin'">
              <xsl:attribute name="type">small road</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'route departementale'">
              <xsl:attribute name="type">medium road</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'route nationale'">
              <xsl:attribute name="type">large road</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'terrain mouille'">
              <xsl:attribute name="type">wet terrain</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'terrain enneige'">
              <xsl:attribute name="type">snowed terrain</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'terrain gele'">
              <xsl:attribute name="type">iced terrain</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'frontiere'">
              <xsl:attribute name="type">border</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mines cluster'">
              <xsl:attribute name="type">mines</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Safe Point'">
              <xsl:attribute name="type">safe zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Jamming Area'">
              <xsl:attribute name="type">jamming area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Full jamming Area'">
              <xsl:attribute name="type">full jamming area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'NBC Contaminated Area - External'">
              <xsl:attribute name="type">nbc zone (external model)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'NBC Cloud - External'">
              <xsl:attribute name="type">nbc cloud (external model)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'NBC Contaminated Area'">
              <xsl:attribute name="type">nbc zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'NBC Cloud'">
              <xsl:attribute name="type">nbc cloud</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Fire'">
              <xsl:attribute name="type">fire</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Wild Surface Fire'">
              <xsl:attribute name="type">surface fire</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Decontamination Site'">
              <xsl:attribute name="type">decontamination site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Supply Route'">
              <xsl:attribute name="type">logistic route</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Crossing Site'">
              <xsl:attribute name="type">crossing site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'ROTA Point'">
              <xsl:attribute name="type">rota area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Abatis'">
              <xsl:attribute name="type">abattis</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Anti-tank Ditch'">
              <xsl:attribute name="type">anti tank obstacle</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Barricade'">
              <xsl:attribute name="type">barricade</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mined Area (linear)'">
              <xsl:attribute name="type">mined area (linear)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mined Area (scattered)'">
              <xsl:attribute name="type">mined area (scattered)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Landslide'">
              <xsl:attribute name="type">landslide</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Roadblock'">
              <xsl:attribute name="type">road destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bridge destruction'">
              <xsl:attribute name="type">bridge destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bridge (continuous)'">
              <xsl:attribute name="type">continuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bridge (discontinuous)'">
              <xsl:attribute name="type">discontinuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Firing post'">
              <xsl:attribute name="type">firing post</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Gun artillery deployment area'">
              <xsl:attribute name="type">gun artillery deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Cobra deployment area'">
              <xsl:attribute name="type">cobra deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mrls deployment area'">
              <xsl:attribute name="type">mrls deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mortar deployment area'">
              <xsl:attribute name="type">mortar deployment area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Rearm, Refuel and Resupply Point (R3P)'">
              <xsl:attribute name="type">logistic support area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Landing Zone'">
              <xsl:attribute name="type">landing area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Airfield Zone'">
              <xsl:attribute name="type">trail</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Platform'">
              <xsl:attribute name="type">platform</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mobility enhanced area'">
              <xsl:attribute name="type">mobility enhanced area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Landing Zone (helicopter)'">
              <xsl:attribute name="type">landing area (helicopter)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Forward Arming and Refueling Point (FARP)'">
              <xsl:attribute name="type">logistic area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Detainee Holding Area'">
              <xsl:attribute name="type">prisoners camp</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Refugee Holding Area'">
              <xsl:attribute name="type">refugees camp</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Checkpoint'">
              <xsl:attribute name="type">checkpoint</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Drop Zone (DZ)'">
              <xsl:attribute name="type">dropping area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'No-Fire Area (NFA)'">
              <xsl:attribute name="type">fire forbidden area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'General obstacle zone'">
              <xsl:attribute name="type">general obstacle zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Installation'">
              <xsl:attribute name="type">installation</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Distribution point'">
              <xsl:attribute name="type">distribution point</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Sensors detection zone'">
              <xsl:attribute name="type">sensorsDetectionZone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Sensors zone'">
              <xsl:attribute name="type">sensors</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Supply Point'">
              <xsl:attribute name="type">supply point</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Urban Block'">
              <xsl:attribute name="type">urban block</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Dirty bomb'">
              <xsl:attribute name="type">dirty bomb</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Hospital'">
              <xsl:attribute name="type">hospital</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Weapons Cache'">
              <xsl:attribute name="type">weapon cache</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Emergency Shelter'">
              <xsl:attribute name="type">emergency shelter</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Meeting Place for Victims (PRV)'">
              <xsl:attribute name="type">emergency meeting place</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Victims Discovery'">
              <xsl:attribute name="type">victim discovery zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Stock'">
              <xsl:attribute name="type">stock</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Flood Origin'">
              <xsl:attribute name="type">flood</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Moving Population'">
              <xsl:attribute name="type">population moving</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Secured Area'">
              <xsl:attribute name="type">secured area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Firebreak'">
              <xsl:attribute name="type">firebreak area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mined Area (linear and destructible)'">
              <xsl:attribute name="type">mined area (linear and destructible)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Dike'">
              <xsl:attribute name="type">dyke</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Underground Exit'">
              <xsl:attribute name="type">underground exit</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Ressource node'">
              <xsl:attribute name="type">ressource node</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Crowd dispersion smoke'">
              <xsl:attribute name="type">Scaterring grenade</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Movement forbidden area'">
              <xsl:attribute name="type">movement forbidden area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Confinement Point'">
                  <xsl:attribute name="type">safe zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Ressource node'">
                  <xsl:attribute name="type">ressource node</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bombe terroriste'">
                  <xsl:attribute name="type">full jamming area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Zone contaminbée NRBC (Source externe)'">
                  <xsl:attribute name="type">nbc zone (external model)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Nuage NRBC (Source externe)'">
                  <xsl:attribute name="type">nbc cloud (external model)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Zone contaminée NRBC'">
                  <xsl:attribute name="type">nbc zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Nuage NRBC'">
                  <xsl:attribute name="type">nbc cloud</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Feu urbain'">
                  <xsl:attribute name="type">fire</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Site de décontamination'">
                  <xsl:attribute name="type">decontamination site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Site de franchissement'">
                  <xsl:attribute name="type">crossing site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Abatis (tempête)'">
                  <xsl:attribute name="type">abattis</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Glissement de terrain'">
                  <xsl:attribute name="type">landslide</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bloc anti émeute'">
                  <xsl:attribute name="type">road destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Destruction de pont'">
                  <xsl:attribute name="type">bridge destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Pont (continu)'">
                  <xsl:attribute name="type">continuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Pont (discontinu)'">
                  <xsl:attribute name="type">discontinuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Point logistique'">
                  <xsl:attribute name="type">logistic support area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Camp de réfugiés'">
                  <xsl:attribute name="type">refugees camp</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Inondation'">
                  <xsl:attribute name="type">flood</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Zone de largage'">
                  <xsl:attribute name="type">dropping area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Zone interdite au tir (NFA)'">
                  <xsl:attribute name="type">fire forbidden area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Point de ravitaillement'">
                  <xsl:attribute name="type">supply point</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bloc urbain'">
                  <xsl:attribute name="type">urban block</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bombe sale'">
                  <xsl:attribute name="type">dirty bomb</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Hôpital'">
                  <xsl:attribute name="type">hospital</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Propagande'">
                  <xsl:attribute name="type">leaflet</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Poste Médical Avancé (PMA)'">
                  <xsl:attribute name="type">Poste Médical Avancé (PMA)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Point de rassemblement des victimes (PRV)'">
                  <xsl:attribute name="type">Point de rassemblement des victimes (PRV)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Captage victimes '">
                  <xsl:attribute name="type">Captage victimes </xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Stock'">
                  <xsl:attribute name="type">Stock</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Feu sauvage'">
                  <xsl:attribute name="type">Feu sauvage</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mouvement Population'">
                  <xsl:attribute name="type">population moving</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Zone securisee'">
                  <xsl:attribute name="type">Zone securisee</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Barrage'">
                  <xsl:attribute name="type">Barrage</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Digue'">
                  <xsl:attribute name="type">dyke</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Noeud de reseau'">
                  <xsl:attribute name="type">Noeud de reseau</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Checkpoint2'">
                  <xsl:attribute name="type">Checkpoint2</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Bombe terroriste 2'">
                  <xsl:attribute name="type">bombe activable</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type = 'Mine Cluster'">
              <xsl:attribute name="type">mines</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Digue'">
                  <xsl:attribute name="type">dyke</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Mouvement Population'">
                  <xsl:attribute name="type">population moving</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Bombe terroriste'">
                  <xsl:attribute name="type">full jamming area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone contaminbée NRBC (Source externe)'">
                  <xsl:attribute name="type">nbc zone (external model)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Nuage NRBC (Source externe)'">
                  <xsl:attribute name="type">nbc cloud (external model)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone contaminée NRBC'">
                  <xsl:attribute name="type">nbc zone</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Nuage NRBC'">
                  <xsl:attribute name="type">nbc cloud</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Feu urbain'">
                  <xsl:attribute name="type">50</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Site de décontamination'">
                  <xsl:attribute name="type">decontamination site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Site de franchissement'">
                  <xsl:attribute name="type">crossing site</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Abatis (tempête)'">
                  <xsl:attribute name="type">abattis</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Barricade'">
                  <xsl:attribute name="type">barricade</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Glissement de terrain'">
                  <xsl:attribute name="type">landslide</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Bloc anti émeute'">
                  <xsl:attribute name="type">road destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Destruction de pont'">
                  <xsl:attribute name="type">50</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Pont (continu)'">
                  <xsl:attribute name="type">continuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Pont (discontinu)'">
                  <xsl:attribute name="type">discontinuous pontoon bridge</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Point logistique'">
                  <xsl:attribute name="type">logistic support area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone d*atterrissage'">
                  <xsl:attribute name="type">trail</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone d*amélioration de déplacement (inondation)'">
                  <xsl:attribute name="type">mobility enhanced area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone d*atterrissage (Hélicoptère)'">
                  <xsl:attribute name="type">landing area (helicopter)</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Camp de réfugiés'">
                  <xsl:attribute name="type">refugees camp</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Inondation'">
                  <xsl:attribute name="type">flood</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Checkpoint'">
                  <xsl:attribute name="type">checkpoint</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone de largage'">
                  <xsl:attribute name="type">dropping area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Zone interdite au tir (NFA)'">
                  <xsl:attribute name="type">fire forbidden area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Point de ravitaillement'">
                  <xsl:attribute name="type">supply point</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Feu urbain'">
                  <xsl:attribute name="type">fire</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Bloc urbain'">
                  <xsl:attribute name="type">urban block</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Bombe sale'">
                  <xsl:attribute name="type">dirty bomb</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Hôpital'">
                  <xsl:attribute name="type">hospital</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Stock'">
                  <xsl:attribute name="type">stock</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Propagande'">
                  <xsl:attribute name="type">leaflet</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Destruction de pont'">
                  <xsl:attribute name="type">bridge destruction</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='Point logistique'">
                  <xsl:attribute name="type">logistic support area</xsl:attribute>
          </xsl:if>
          <xsl:if test="./@type ='General obstacle zone'">
                  <xsl:attribute name="type">movement forbidden area</xsl:attribute>
          </xsl:if>
      </xsl:copy>
  </xsl:template>

  <xsl:template match="/missions">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
      <xsl:if test="count( ./translation-queries ) = 0">
        <xsl:element name="translation-queries">
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/units/mission/string(@name)</xsl:attribute>
            <xsl:attribute name="context">unit-missions</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/units/mission/parameter/string(@name)</xsl:attribute>
            <xsl:attribute name="context">unit-missions-parameters</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/automats/mission/string(@name)</xsl:attribute>
            <xsl:attribute name="context">automat-missions</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/automats/mission/parameter/string(@name)</xsl:attribute>
            <xsl:attribute name="context">automat-parameters</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/populations/mission/string(@name)</xsl:attribute>
            <xsl:attribute name="context">crowd-missions</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/populations/mission/parameter/string(@name)</xsl:attribute>
            <xsl:attribute name="context">crowd-parameters</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/fragorders/fragorder/string(@name)</xsl:attribute>
            <xsl:attribute name="context">frag-order</xsl:attribute>
          </xsl:element>
          <xsl:element name="translation-query">
            <xsl:attribute name="query">/missions/fragorders/fragorder/parameter/string(@name)</xsl:attribute>
            <xsl:attribute name="context">frag-order-parameters</xsl:attribute>
          </xsl:element>
        </xsl:element>
      </xsl:if>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>
  
</xsl:stylesheet>

