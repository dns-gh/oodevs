using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterIntelligences : OrderParameter
        {
            sealed class ParameterTypeIntelligence : ParameterType_ABC
            {
                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    throw new Exception("Intelligence.OnContextMenu should not be here.");
                }

                public override string Type
                {
                    get
                    {
                        return "IntelligenceList";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return "null";
                    }
                }
            }

            public ParameterIntelligences()
                : base("Intelligences", new ParameterTypeIntelligence())
            {
                // NOTHING
            }
        }
    }
}
