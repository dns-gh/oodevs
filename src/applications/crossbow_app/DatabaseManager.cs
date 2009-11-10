using System.Text.RegularExpressions;
using System.Collections.Generic;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.DataSourcesGDB;
using ESRI.ArcGIS.ArcGlobe;
using ESRI.ArcGIS.DefenseSolutions;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.esriSystem;

namespace Sword
{
    namespace Crossbow
    {
        public class DatabaseManager
        {
            private Dictionary<string, IWorkspace> m_workspaces = new Dictionary<string, IWorkspace>();

            public DatabaseManager()
            {
            }

            private static bool IsFileDatabase(string file)
            {
                string ext = file.Substring(file.LastIndexOf('.') + 1, 3);
                return ext == "mdb" || ext == "gdb";
            }

            private static bool IsConnectionDatabase(string file)
            {
                return file.Substring(0, 6) == "sde://";                
            }

            private static IWorkspace OpenFileDatabase(string file)
            {
                IWorkspaceFactory factory = null;
                string ext = file.Substring(file.LastIndexOf('.') + 1, 3);
                if (ext == "mdb")
                    factory = new AccessWorkspaceFactoryClass();
                if (ext == "gdb")
                    factory = new FileGDBWorkspaceFactoryClass();
                if (factory != null)
                    return factory.OpenFromFile(file, 0);
                return null;
            }

            private static IWorkspace OpenConnectionDatabase(string file)
            {
                Regex expression = new Regex("(sde)://(?<user>\\w+):(?<password>\\w+)@(?<host>\\w+)(:(?<port>\\d+)){0,1}/(?<database>\\w+).(?<schema>\\w*)");
                
                Match match = expression.Match(file);
                if (expression.IsMatch(file))
                {
                    IWorkspaceFactory factory = new SdeWorkspaceFactoryClass();
                    IPropertySet property = new PropertySetClass();

                    string protocol = match.Groups[1].Value;
                    property.SetProperty("server", match.Groups["host"].Value);
                    property.SetProperty("instance", match.Groups[1].Value + ":postgresql:" + match.Groups["host"].Value);
                    property.SetProperty("user", match.Groups["user"].Value);
                    property.SetProperty("password", match.Groups["password"].Value);
                    if (match.Groups["port"].Value != "")
                        property.SetProperty("port", match.Groups["port"].Value);
                    property.SetProperty("database", match.Groups["database"].Value);
                    property.SetProperty("version", "sde.DEFAULT");
                    if (factory != null)
                        return factory.Open(property, 0);
                }
                return null;
            }

            ///<summary>
            /// Open a valid workspace according to the database's extension
            ///</summary>
            ///<param name="file">Name of the workspace</param>
            public IWorkspace OpenWorkspace(string file)
            {
                if (IsFileDatabase(file))
                    m_workspaces[ file ] = OpenFileDatabase(Tools.GetExtension().Config.ExercisePath + "/" + file);
                if (IsConnectionDatabase(file))
                    m_workspaces[file] = OpenConnectionDatabase(file);
                return m_workspaces[file];
            }

            public IWorkspace GetWorkspace(string file)
            {                
                if (!m_workspaces.ContainsKey(file))
                    return OpenWorkspace(file);
                else
                    return m_workspaces[file];
            }

            #region "Database manipulation"
            public static void SetValue<T>(IRowBuffer row, string field, T value)
            {
                try
                {
                    int id = row.Fields.FindField(field);
                    if (id >= 0 && row.Fields.get_Field(id).Editable)
                        row.set_Value(id, value);
                }
                catch (System.Exception e)
                {
                    System.Diagnostics.Trace.WriteLine(e.Message);
                }
            }

            public static void SetValue<T>(IRow row, string field, T value)
            {
                try
                {
                    int id = row.Fields.FindField(field);
                    if (id >= 0 && row.Fields.get_Field(id).Editable)
                        row.set_Value(id, value);
                }
                catch (System.Exception e)
                {
                    System.Diagnostics.Trace.WriteLine(e.Message);
                }
            }

            /// <summary>
            ///  Retrieve value from a specific data field
            /// </summary>
            /// <typeparam name="T">Value type</typeparam>
            /// <param name="row">row</param>
            /// <param name="field">field name</param>
            /// <returns>Value of the field</returns>
            public static T GetValue<T>(IRow row, string field)
            {
                int id = row.Fields.FindField(field);
                if (id >= 0)
                    return (T)row.get_Value(id);
                return default(T);
            }

            /// <summary>
            ///  Store geometry into table. Create a new feature and store input geometry
            /// </summary>
            /// <param name="table">Name of the output table</param>
            /// <param name="value">Input geometry</param>
            /// <remarks>Input geometry should be zAwared according to the table</remarks>
            public void Store(string workspace, string table, IGeometry value)
            {
                try
                {
                    IFeatureWorkspace ws = (IFeatureWorkspace)GetWorkspace(workspace);
                    ScopeLockEditor locker = new ScopeLockEditor(ws);
                    locker.Lock();
                    IFeatureClass features = ws.OpenFeatureClass(table);
                    IFeature feature = features.CreateFeature();
                    feature.Shape = value;
                    feature.Store();
                    locker.Unlock();
                }
                catch (System.Exception e)
                {
                    System.Diagnostics.Trace.WriteLine(e.Message);
                }
            }

            /// <summary>
            ///  Remove all features of the specified feature class
            /// </summary>
            /// <param name="workspace">Workspace</param>
            /// <param name="name">Feature class</param>
            public static void ClearClass(IWorkspace workspace, string name)
            {
                try
                {
                    IFeatureWorkspace fws = (IFeatureWorkspace)workspace;
                    if (fws == null)
                        return;
                    ITable table = (ITable)fws.OpenFeatureClass(name);
                    if (table != null)
                    {
                        IQueryFilter filter = null;
                        table.DeleteSearchedRows(filter);
                    }
                }
                catch (System.Exception e)
                {
                    System.Diagnostics.Trace.WriteLine(e.Message);
                }
            }
            #endregion
        }
    }
}