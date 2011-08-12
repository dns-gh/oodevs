<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="1.0">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="text()"/>
	<xsl:template match="/">
		<objects>
			<xsl:apply-templates/>
		</objects>
	</xsl:template>
	
   <xsl:template match="object">
        <xsl:copy>
			
			<xsl:copy-of select="node()|@*"/>
			<xsl:apply-templates/>
			
 
			<xsl:if test="@can-be-maneuver-obstacle">
				<activable/>
			</xsl:if>
				<xsl:if test="@avoid-distance">
					<avoidable>
						<xsl:attribute name="distance">
							<xsl:value-of select="@avoid-distance"/>
						</xsl:attribute>
					</avoidable>
				</xsl:if>
				<xsl:if test="@default-bypassed-speed">
					<bypassable>
						<xsl:attribute name="bypass-speed">
							<xsl:value-of select="@default-bypassed-speed"/>
						</xsl:attribute>
					</bypassable>
				</xsl:if>
			    <xsl:if test="@default-consumption-mode">
				    <constructor>
						<xsl:attribute name=" default-consumption-mode">
							<xsl:value-of select="@default-consumption-mode"/>
						</xsl:attribute>
						<xsl:attribute name=" unit-type">raw</xsl:attribute>
						<xsl:if test="dotations/construction/@count">									
							<buildable>
								<resources>
									<resource>
										<xsl:attribute name="count">
											<xsl:value-of select="dotations/construction/@count"/>
										</xsl:attribute>
										<xsl:attribute name="name">
											<xsl:value-of select="dotations/construction/@dotation"/>
										</xsl:attribute>
									</resource>
								</resources>
							</buildable>
    						</xsl:if>
							<xsl:if test="dotations/valorization/@count">		
							<buildable>
								<resources>
									<resource>
										<xsl:attribute name="count">
											<xsl:value-of select="dotations/valorization/@count"/>
										</xsl:attribute>
										<xsl:attribute name="name">
											<xsl:value-of select="dotations/valorization/@dotation"/>
										</xsl:attribute>
									</resource>
								</resources>
							</buildable>
						</xsl:if>
					</constructor>
				</xsl:if>
				<xsl:if test="@default-speed">
					<mobility>
						<xsl:attribute name="default-speed">
							<xsl:value-of select="@default-speed"/>
						</xsl:attribute>
						<xsl:attribute name="max-unit-percentage-speed">
							<xsl:value-of select="@max-unit-percentage-speed"/>
						</xsl:attribute>
						<xsl:attribute name="unit-speed-impact-mode">
							<xsl:value-of select="@unit-speed-impact-mode"/>
						</xsl:attribute>
					</mobility>
				</xsl:if>
			    <xsl:if test="@population-density">
					<population-filter>
						<xsl:attribute name="density">
							<xsl:value-of select="@population-density"/>
						</xsl:attribute>
					</population-filter>
					</xsl:if>
			</xsl:copy>		
    </xsl:template>
</xsl:transform>