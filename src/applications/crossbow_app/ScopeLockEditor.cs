using ESRI.ArcGIS.Geodatabase;


namespace Crossbow
{
    /*
     * Enable IFeatureWorkspace edition during instance life
     */
    class ScopeLockEditor
    {
        IWorkspaceEdit m_edit;
        bool m_IsBeingEdited;

        public ScopeLockEditor(IFeatureWorkspace featureWorkspace)
        {
            m_edit = (IWorkspaceEdit)featureWorkspace;
        }
        public void Lock()
        {
            if (!m_edit.IsBeingEdited())
                m_edit.StartEditing(true);
            m_edit.StartEditOperation();
        }
        public void Abord()
        {
            m_edit.AbortEditOperation();
        }
        public void Unlock()
        {
            m_edit.StopEditOperation();
            if (m_edit.IsBeingEdited())
                m_edit.StopEditing(true);
        }
    }
}