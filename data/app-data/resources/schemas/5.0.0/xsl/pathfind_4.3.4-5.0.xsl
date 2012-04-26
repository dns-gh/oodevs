<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

    <xsl:template match="unit-rules">
      <xsl:copy>
        <xsl:apply-templates select="node()|@*"/>
        <xsl:if test="not(rule/@type='nbc')">
          <rule autonomous="false" flying="false" type="nbc">
              <optimisation shortest="false"/>
              <preferred-altitude value="0"/>
              <preferred-terrains strength="50">
                <preferred-terrain type="nbc zone"/>
                <preferred-terrain type="nbc cloud"/>
              </preferred-terrains>
              <avoided-terrains strength="7"/>
              <zone>
                <outter-tolerance cost-per-meter="1" with-automat="10000" without-automat="1000"/>
                <inner-comfort cost-per-meter="0.002" distance="500"/>
              </zone>
              <automat-zone>
                <outter-tolerance cost-per-meter="0" distance="0"/>
              </automat-zone>
              <danger-direction base-cost-beyond="1" cost-per-meter="0.001"/>
              <enemies cost-at-security-range="0" cost-on-contact="0"/>
              <population-costs maximum="100" security-range="50">
                <population-cost attitude="calme" contact-cost="10"/>
                <population-cost attitude="agitee" contact-cost="10"/>
                <population-cost attitude="excitee" contact-cost="50"/>
                <population-cost attitude="agressive" contact-cost="50"/>
              </population-costs>
              <object-costs avoid="true" threshold="10000">
                <object-cost type="anti tank obstacle" value="10000"/>
                <object-cost type="abattis" value="10000"/>
                <object-cost type="barricade" value="10000"/>
                <object-cost type="mines" value="10000"/>
                <object-cost type="mined area (linear)" value="10000"/>
                <object-cost type="mined area (scattered)" value="10000"/>
                <object-cost type="landslide" value="10000"/>
                <object-cost type="road destruction" value="10000"/>
                <object-cost type="bridge destruction" value="10000"/>
                <object-cost type="continuous pontoon bridge" value="0"/>
                <object-cost type="discontinuous pontoon bridge" value="0"/>
                <object-cost type="firing post" value="0"/>
                <object-cost type="safe zone" value="0"/>
                <object-cost type="gun artillery deployment area" value="10"/>
                <object-cost type="cobra deployment area" value="10"/>
                <object-cost type="mrls deployment area" value="10"/>
                <object-cost type="crossing site" value="10"/>
                <object-cost type="decontamination site" value="10"/>
                <object-cost type="logistic support area" value="10"/>
                <object-cost type="jamming area" value="10000"/>
                <object-cost type="rota area" value="10000"/>
                <object-cost type="landing area" value="10"/>
                <object-cost type="trail" value="-5"/>
                <object-cost type="platform" value="10"/>
                <object-cost type="mobility enhanced area" value="10"/>
                <object-cost type="landing area (helicopter)" value="10"/>
                <object-cost type="logistic area" value="10"/>
                <object-cost type="prisoners camp" value="10"/>
                <object-cost type="refugees camp" value="10"/>
                <object-cost type="checkpoint" value="-5"/>
                <object-cost type="dropping area" value="10"/>
                <object-cost type="movement forbidden area" value="10000"/>
                <object-cost type="mortar deployment area" value="10"/>
                <object-cost type="installation" value="0"/>
                <object-cost type="urban block" value="10000"/>
                <object-cost type="seal off area" value="10000"/>
                <object-cost type="nbc zone" value="0"/>
                <object-cost type="nbc cloud" value="0"/>
              </object-costs>
            </rule>
            <rule autonomous="false" flying="true" inherits="base" type="nbc"/>
            <rule autonomous="true" flying="false" inherits="base" type="nbc"/>
            <rule autonomous="true" flying="true" inherits="base" type="nbc"/>
        </xsl:if>>
      </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
    
