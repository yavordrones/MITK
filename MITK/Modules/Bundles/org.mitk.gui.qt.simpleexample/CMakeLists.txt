PROJECT(org_mitk_gui_qt_simpleexample)

MACRO_CREATE_MITK_CTK_PLUGIN(
  EXPORT_DIRECTIVE SIMPLEEXAMPLE_EXPORTS
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDENCIES QmitkExt
)
