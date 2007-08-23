<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="FacteursHumains">
		<humans-factors>
			<xsl:for-each select="./*">
				<xsl:if test="name() = 'Experience'">
					<xsl:element name="experience-factor">
						<xsl:for-each select="./*">
							<modifier state="{name()}" max-speed="{CoefModificationVitesseMax}" ph="{CoefModificationPH}" sensor-distance="{CoefModificationDistanceCapteurs}" loading-time="{CoefModificationTempsRechargement}" posture-setup-time="{CoefModificationTempsMiseEnPosture}"/>
						</xsl:for-each>
					</xsl:element>
				</xsl:if>
				<xsl:if test="name() = 'Fatigue'">
					<xsl:element name="tiredness-factor">
						<xsl:for-each select="./*">
							<modifier state="{name()}" max-speed="{CoefModificationVitesseMax}" ph="{CoefModificationPH}" sensor-distance="{CoefModificationDistanceCapteurs}" loading-time="{CoefModificationTempsRechargement}" posture-setup-time="{CoefModificationTempsMiseEnPosture}"/>
						</xsl:for-each>
					</xsl:element>
				</xsl:if>
			</xsl:for-each>
		</humans-factors>
	</xsl:template>
	
</xsl:stylesheet>
