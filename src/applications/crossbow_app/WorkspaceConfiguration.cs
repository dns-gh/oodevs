using System;
using System.Collections;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using ESRI.ArcGIS.Geodatabase;

namespace Sword
{
    namespace Crossbow
    {
        #region FeatureLayerList
        public sealed class FeatureLayerList : StringConverter
        {
            private static ArrayList m_values = new ArrayList();

            public static void SetValues(IEnumDatasetName names)
            {
                m_values.Clear();
                IDatasetName name;
                while ((name = names.Next()) != null)
                    m_values.Add(name.Name);
            }

            #region StringConverter override
            public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
            {
                return new StandardValuesCollection(m_values);
            }

            public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
            {
                return true;
            }
            #endregion
        }
        #endregion

        #region WorkspaceConfiguration
        public sealed class WorkspaceConfiguration
        {
            private LayersConfiguration m_layers = new LayersConfiguration();
            private string m_exercisePath;
            private string m_exerciseFile;
            private string m_workspaceFile;
            private string m_sharedFile;
            private string m_populationFile;

            public event EventHandler ConfigurationLoaded;

            public void NotifyLoaded()
            {
                ConfigurationLoaded(this, EventArgs.Empty);
            }

            public string ExercisePath
            {
                get
                {
                    return m_exercisePath;
                }
                set
                {
                    m_exercisePath = value;
                }
            }

            public string ExerciseFile
            {
                get
                {
                    return m_exerciseFile;
                }
                set
                {
                    m_exerciseFile = value;
                }
            }

            private static string ExtractDirectory(string filename)
            {
                return Path.GetDirectoryName(filename);
            }

            public string BuildChildFile(string filename)
            {
                return Path.GetFullPath(ExtractDirectory(m_exerciseFile) + "/" + filename);
            }

            public string WorkspaceFile
            {
                get
                {
                    return m_workspaceFile;
                }
                set
                {
                    m_workspaceFile = value;
                }
            }

            public string SharedFile
            {
                get
                {
                    return m_sharedFile;
                }
                set
                {
                    m_sharedFile = value;
                }
            }

            public string PopulationFile
            {
                get
                {
                    return m_populationFile;
                }
                set
                {
                    m_populationFile = value;
                }
            }

            public LayersConfiguration LayersConfiguration
            {
                get
                {
                    return m_layers;
                }
            }
        }
        #endregion

        #region LayersConfiguration
        public sealed class LayersConfiguration
        {
            private string m_orders = "Orders";
            private string m_orderparameters = "OrderParameters";            
            private string m_orderparameter_point = "OrderParameters_Point";
            private string m_orderparameter_line = "OrderParameters_Line";
            private string m_orderparameter_area = "OrderParameters_Area";
            private string m_formations = "Formations";
            private string m_units = "UnitForces";
            private string m_knowledgeunits = "KnowledgeUnits";
            private string m_limits = "BoundaryLimits";
            private string m_limas = "TacticalLines";
            private string m_reports = "Reports";
            private string m_objectsPoint = "TacticalObject_Point";

            const string database_ = "sword_bruxelles_db";
            const string schema_ = "sword";

            private string Decorate(string table)
            {
                return database_ + "." + schema_ + "." + table;
            }

            public string Orders
            {
                get
                {
                    return Decorate(m_orders);
                }
            }

            public string OrderParameters
            {
                get
                {
                    return Decorate(m_orderparameters);
                }
            }

            public string OrderParameterPoint
            {
                get
                {
                    return Decorate(m_orderparameter_point);
                }
            }

            public string OrderParameterLine
            {
                get
                {
                    return Decorate(m_orderparameter_line);
                }
            }

            public string OrderParameterArea
            {
                get
                {
                    return Decorate(m_orderparameter_area);
                }
            }

            public string Formations
            {
                get
                {
                    return Decorate(m_formations);
                }
            }

            [DescriptionAttribute("Layer containing units")]
            [CategoryAttribute("Layer bindings")]
            [TypeConverterAttribute(typeof(FeatureLayerList))]
            [DefaultValue("UnitForces")]
            public string Units
            {
                get
                {
                    return Decorate( m_units );
                }
                set
                {
                    m_units = value;
                }
            }

            [DescriptionAttribute("Layer containing knowledges")]
            [CategoryAttribute("Layer bindings")]
            [TypeConverterAttribute(typeof(FeatureLayerList))]
            [DefaultValue("KnowledgeUnits")]
            public string KnowledgeUnits
            {
                get
                {
                    return Decorate(m_knowledgeunits);
                }
                set
                {
                    m_knowledgeunits = value;
                }
            }

            [DescriptionAttribute("Layer containing tactical lines")]
            [CategoryAttribute("Layer bindings")]
            [TypeConverterAttribute(typeof(FeatureLayerList))]
            [DefaultValue("TacticalLines")]
            public string Limas
            {
                get
                {
                    return Decorate( m_limas );
                }
                set
                {
                    m_limas = value;
                }
            }

            [DescriptionAttribute("Layer containing boundary limits")]
            [CategoryAttribute("Layer bindings")]
            [TypeConverterAttribute(typeof(FeatureLayerList))]
            [DefaultValue("BoundaryLimits")]
            public string Limits
            {
                get
                {
                    return Decorate( m_limits );
                }
                set
                {
                    m_limits = value;
                }
            }

            [DescriptionAttribute("Layer containing tactical objects")]
            [CategoryAttribute("Layer bindings")]
            [TypeConverterAttribute(typeof(FeatureLayerList))]
            [DefaultValue("TacticalObjectPoint")]
            public string TacticalObjectPoint
            {
                get
                {
                    return Decorate( m_objectsPoint );
                }
                set
                {
                    m_objectsPoint = value;
                }
            }

            [DescriptionAttribute("Table containing reports")]
            [CategoryAttribute("Table bindings")]
            [TypeConverterAttribute(typeof(FeatureLayerList))]
            [DefaultValue("Reports")]
            public string Reports
            {
                get
                {
                    return Decorate( m_reports );
                }
                set
                {
                    m_reports = value;
                }
            }

        }
        #endregion
    }
}
